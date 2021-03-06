#include "stdafx.h"
#include "lcpp/cli/interpreter.h"
#include "lcpp/cli/ioUtils.h"
#include <iostream>
#include <string>

#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/printer.h"
#include "lcpp/core/ioUtils.h"

#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/void.h"

#include "lcpp/core/exceptions/readerException.h"
#include "lcpp/core/exceptions/evaluatorException.h"
#include "lcpp/core/exceptions/exitException.h"

#include "lcpp/core/exceptions/fileException.h"

static bool g_debugBreakOnException = false;
static lcpp::LispRuntimeState* g_pRunTime = nullptr;

EZ_ON_GLOBAL_EVENT(ThrowException)
{
    auto pException = (lcpp::exceptions::ExceptionBase*)param0.Get<void*>();
    auto szMessage = pException->what();
}

namespace lcpp
{
#if EZ_ENABLED(EZ_PLATFORM_WINDOWS)
    static BOOL ConsoleCtrlHandler(DWORD dwControlType)
    {
        switch(dwControlType)
        {
        case CTRL_C_EVENT:
            if(g_pRunTime)
                g_pRunTime->setBreakExecution(true);
            return TRUE;
        }

        return FALSE;
    }
#endif

    Interpreter::Interpreter() :
        m_state(),
        m_pState(&m_state),
        m_out(std::cout),
        m_in(std::cin),
        m_userPrompt(),
        m_readerBuffer()
    {
    }

    Interpreter::~Interpreter()
    {
    }

    void Interpreter::initialize()
    {
        m_pState->initialize();
        g_pRunTime = m_pState.get();

        ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);

        ezStringBuilder workingDir;
        getCurrentWorkingDirectory(workingDir);

        auto dataDir = ezStringBuilder();
        dataDir.AppendPath(workingDir.GetData(), "data1");
        dataDir.MakeCleanPath();

        // Base dir
        //////////////////////////////////////////////////////////////////////////
        {
            ezStringBuilder baseDir(dataDir);
            baseDir.AppendPath("base");

            auto result = ezFileSystem::AddDataDirectory(baseDir.GetData(), ezFileSystem::ReadOnly, "base-data");

            if(!result.Succeeded())
            {
                baseDir.Prepend("Unable to add base dir: ");

                throw std::exception(baseDir.GetData());
            }

            m_pState->setBaseDirectory(baseDir.GetData());
        }

        // User dir
        //////////////////////////////////////////////////////////////////////////
        {
            ezStringBuilder userDir(dataDir);
            userDir.AppendPath("user");

            auto result = ezFileSystem::AddDataDirectory(userDir.GetData(), ezFileSystem::ReadOnly, "user-data");

            if(!result.Succeeded())
            {
                userDir.Prepend("Unable to add user dir: ");

                throw std::exception(userDir.GetData());
            }

            m_pState->setUserDirectory(userDir.GetData());
        }

        m_userPrompt.Clear();

#if EZ_ENABLED(EZ_PLATFORM_WINDOWS)
        {
            // Register the console Ctrl handler.
            auto bSuccess = SetConsoleCtrlHandler((PHANDLER_ROUTINE) ConsoleCtrlHandler, TRUE);
            if(bSuccess == FALSE)
            {
                ezLog::Info("Failed to register console Ctrl handler.");
            }
        }
#endif
    }

    void Interpreter::shutdown()
    {
        g_pRunTime = nullptr;
#if EZ_ENABLED(EZ_PLATFORM_WINDOWS)
        // Remove the console Ctrl handler.
        SetConsoleCtrlHandler((PHANDLER_ROUTINE) ConsoleCtrlHandler, FALSE);
#endif

        m_userPrompt.Clear();

        m_pState->shutdown();
    }

    ezInt32 Interpreter::repl()
    {
        evalInitFile();

        std::ios_base::sync_with_stdio(false);

        ezDeque<Ptr<LispObject>> results(lcpp::defaultAllocator());

        ezUInt32 currentLine(0);

        auto& syntaxCheck = m_pState->getReaderState()->m_syntaxCheckResult;
        auto& outputStream = *m_pState->getPrinterState()->m_pOutStream;

        outputStream << "=== Scheme interpreter 'lcpp' ===\n";

        while(true)
        {
            ++currentLine;

            syntaxCheck.reset();
            m_readerBuffer.Clear();
            EZ_ASSERT(results.IsEmpty(), "");

            prepareUserPrompt(outputStream, false, currentLine);

            try
            {
                LCPP_SCOPE_EXIT
                {
                    while(!results.IsEmpty())
                    {
                        m_pState->getGarabgeCollector()->removeRoot(results.PeekFront().get());
                        results.PopFront();
                    }
                };
                auto uiNumReadLines = readUserInput(results);
                if(m_in.eof())
                {
                    lineBreak(outputStream);
                    continue;
                }
                currentLine += uiNumReadLines - 1;

                while(!results.IsEmpty())
                {
                    StackPtr<LispObject> pResult = results.PeekFront();
                    m_pState->getGarabgeCollector()->removeRoot(results.PeekFront().get());
                    results.PopFront();

                    auto pToPrint = evaluateReaderOutput(pResult);

                    if(isVoid(pToPrint))
                    {
                        continue;
                    }

                    print(pToPrint);
                    lineBreak(outputStream);
                }

                continue;
            }
            catch(exceptions::ReaderBase& ex)
            {
                const auto& sourcePos = syntaxCheck.m_cursor.getPosition();

                auto info = ezStringBuilder();

                for(auto i = m_userPrompt.GetCharacterCount() + sourcePos.m_column; i > 0; --i)
                {
                    info.Append('-');
                }
                info.Append('^');
                lineBreak(info);

                info.AppendFormat("Parser error in stdin(%u:%u): %s", sourcePos.m_line + currentLine, sourcePos.m_column, ex.what());

                outputStream << info.GetData();
            }
            catch(exceptions::EvaluatorBase& ex)
            {
                auto info = ezStringBuilder();
                info.Format("Evaluator error: %s", ex.what());
                outputStream << info.GetData();
            }
            catch(exceptions::Exit& ex)
            {
                outputStream << ex.what();
                return ex.getExitCode();
            }
            catch(exceptions::ExceptionBase& ex)
            {
                outputStream << "Error: " << ex.what();
            }
            catch(std::exception& ex)
            {
                outputStream << "Unexpected error: " << ex.what();
            }
            catch(...)
            {
                outputStream << "Fatal, unknown error occurred.";
            }

            lineBreak(outputStream);
        }
    }

    void Interpreter::evalInitFile()
    {
        StackPtr<LispObject> pContMain = cont::createTopLevel(m_pState);

        StackPtr<LispObject> pContEval = cont::create(pContMain, &eval::evaluate);
        cont::getStack(pContEval)->push(m_pState->getGlobalEnvironment());
        // eval::evaluate needs a second argument, the object to evaluate, which will be provided by reader::read.

        StackPtr<LispObject> pContRead = cont::create(pContEval, &reader::read);
        auto& outputStream = *m_pState->getPrinterState()->m_pOutStream;

        //////////////////////////////////////////////////////////////////////////

        StackPtr<LispObject> pStream = LCPP_pNil;

        {
            ezStringBuilder absoluteFileName("init.lisp");
            absoluteFileName.MakeAbsolutePath(m_pState->getBaseDirectory());

            ezFileReader fileReader;
            auto openingTheFile = fileReader.Open(absoluteFileName.GetData());

            if(openingTheFile.Failed())
            {
                ezStringBuilder message;
                message.AppendFormat("Failed to open file \"%s\".", absoluteFileName.GetData());
                LCPP_THROW(exceptions::UnableToOpenFile(message.GetData()));
            }

            auto fileSize64 = fileReader.GetFileSize();
            auto fileSize32 = ezUInt32(fileSize64);

            ezHybridArray<ezUInt8, 256> rawFileContent;
            rawFileContent.SetCount(fileSize32 + 1);
            fileReader.ReadBytes(&rawFileContent[0], fileSize32);

            rawFileContent[fileSize32] = '\0';

            pStream = stream::create(str::create((const char*)&rawFileContent[0], fileSize32));
        }

        //////////////////////////////////////////////////////////////////////////

        typeCheck(pStream, Type::Stream);
        cont::getStack(pContRead)->push(pStream);

        while(true)
        {
            try
            {
                cont::trampoline(pContRead);
            }
            catch(exceptions::ExceptionBase& ex)
            {
                outputStream << "Error: " << ex.what();
            }
            catch(std::exception& ex)
            {
                outputStream << "Unexpected error: " << ex.what();
            }
            catch(...)
            {
                outputStream << "Fatal, unknown error occurred.";
            }

            if (!stream::isValid(pStream))
            {
                break;
            }

            cont::setFunction(pContEval, &eval::evaluate);
            cont::getStack(pContEval)->clear();
            cont::getStack(pContEval)->push(m_pState->getGlobalEnvironment());

            cont::setFunction(pContRead, &reader::read);
            cont::getStack(pContRead)->clear();
            cont::getStack(pContRead)->push(pStream);
        }

        // Check pStackMain.
        return;
    }

    ezUInt32 Interpreter::readUserInput(ezDeque<Ptr<LispObject>>& out_results)
    {
        StackPtr<LispObject> pContMain = cont::createTopLevel(m_pState);

        StackPtr<LispObject> pContRead = cont::create(pContMain, &reader::read);

        std::string inputBuffer;
        StackPtr<LispObject> pInputString = str::create("");
        StackPtr<LispObject> pStream = stream::create(pInputString);
        auto& syntaxCheck = m_pState->getReaderState()->m_syntaxCheckResult;

        ezUInt32 uiNumUserInputLines(1);

        // Multiline input, e.g. (define (fac n)
        //                         (if (<= n 1)
        //                           1
        //                           (* n (fac (- n 1)))))
        while(true)
        {
            try
            {
                m_in.clear(); // Clear flags
                std::getline(m_in, inputBuffer);
                if(m_in.eof()) { return 1; }
                m_readerBuffer.AppendFormat("%s", inputBuffer.c_str());
                pInputString = str::create(m_readerBuffer.GetIteratorFront());
                pStream = stream::create(pInputString);

                // Multiple objects per line, e.g.: "(fac 1)  2  (isPrime 42)"
                while(true)
                {
                    cont::getStack(pContMain)->clear();
                    cont::getStack(pContRead)->clear();

                    cont::getStack(pContRead)->push(pStream);

                    syntaxCheck.reset();

                    cont::setFunction(pContRead, &reader::read);
                    cont::trampoline(pContRead);

                    if(syntaxCheck.m_isPureWhitespace)
                    {
                        break;
                    }

                    auto pResult = cont::getStack(pContMain)->get(0);
                    out_results.PushBack(pResult);
                    m_pState->getGarabgeCollector()->addRoot(out_results.PeekBack().get());

                    {
                        auto& iterator = stream::getIterator(pStream);
                        m_readerBuffer = iterator;
                        pInputString = str::create(m_readerBuffer.GetIteratorFront());
                        pStream = stream::create(pInputString);
                    }

                    if(!stream::isValid(pStream))
                    {
                        break;
                    }
                }
            }
            catch(exceptions::MissingRightListDelimiter&)
            {
                lineBreak(m_readerBuffer);
                ++uiNumUserInputLines;
                addPadding(*m_pState->getPrinterState()->m_pOutStream, '.');
                continue;
            }
            // If we reach this point, no exception was thrown and everything is fine.
            break;
        }

        return uiNumUserInputLines;
    }

    Ptr<LispObject> Interpreter::evaluateReaderOutput(StackPtr<LispObject> pObject)
    {
        StackPtr<LispObject> pContMain = cont::createTopLevel(m_pState);

        StackPtr<LispObject> pContEvaluate = cont::create(pContMain, &eval::evaluate);
        cont::getStack(pContEvaluate)->push(m_pState->getGlobalEnvironment());
        cont::getStack(pContEvaluate)->push(pObject);

        cont::trampoline(pContEvaluate);

        return cont::getStack(pContMain)->get(0);
    }

    void Interpreter::print(StackPtr<LispObject> pToPrint)
    {
        StackPtr<LispObject> pContMain = cont::createTopLevel(m_pState);

        StackPtr<LispObject> pContRead = cont::create(pContMain, &printer::print);
        cont::getStack(pContRead)->push(pToPrint);

        cont::trampoline(pContRead);
    }

    void Interpreter::prepareUserPrompt(ezStreamWriterBase& outputStream, bool printNewLine, ezUInt32 currentLine)
    {
        if(printNewLine)
        {
            lineBreak(outputStream);
        }

        m_userPrompt.Format("%u> ", currentLine);
        outputStream << m_userPrompt;
    }

    void Interpreter::addPadding(ezStringBuilder& builder, ezUInt32 paddingCharacter)
    {
        const auto maxCount = m_userPrompt.GetCharacterCount() - 1;
        for(ezUInt32 count = 0; count < maxCount; ++count)
        {
            builder.Append(paddingCharacter);
        }
        builder.Append(' ');
    }

    void Interpreter::addPadding(ezStreamWriterBase& outputStream, ezUInt32 paddingCharacter)
    {
        auto output = ezStringBuilder();
        addPadding(output, paddingCharacter);
        outputStream << output;
    }

    void Interpreter::lineBreak(ezStringBuilder& builder)
    {
        builder.Append(m_pState->getReaderState()->m_newLineDelimiter);
    }

    void Interpreter::lineBreak(ezStreamWriterBase& outputStream)
    {
        ezStringBuilder output;
        lineBreak(output);
        outputStream << output;
    }

}
