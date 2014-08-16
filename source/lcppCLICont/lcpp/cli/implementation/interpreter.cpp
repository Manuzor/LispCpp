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


namespace lcpp
{
    Interpreter::Interpreter() :
        m_state(),
        m_pState(&m_state),
        m_out(std::cout),
        m_in(std::cin),
        m_szDataDir(),
        m_userPrompt(),
        m_readerBuffer()
    {
    }

    Interpreter::~Interpreter()
    {
    }

    void Interpreter::initialize()
    {
        ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);

        auto dataDir = ezStringBuilder();
        auto applicationDir = getCurrentWorkingDirectory();
        dataDir.AppendPath(applicationDir.GetData(), "data");
        dataDir.MakeCleanPath();
        auto result = ezFileSystem::AddDataDirectory(dataDir.GetData(), ezFileSystem::ReadOnly, "data");

        if(!result.Succeeded())
        {
            dataDir.Prepend("Unable to add data dir: ");
            throw std::exception(dataDir.GetData());
        }
        m_szDataDir = dataDir;

        m_pState->initialize();

        m_userPrompt.Clear();
    }

    void Interpreter::shutdown()
    {
        m_userPrompt.Clear();

        m_pState->shutdown();
    }

    ezInt32 Interpreter::repl()
    {
        std::ios_base::sync_with_stdio(false);

        auto results = ezDeque<Ptr<LispObject>>(lcpp::defaultAllocator());

        auto inputBuffer = std::string("");

        auto currentLine = ezUInt32(0);

        auto& syntaxCheck = m_pState->getReaderState()->m_syntaxCheckResult;
        auto& outputStream = *m_pState->getPrinterState()->m_pOutStream;

        outputStream << "=== Scheme interpreter 'lcpp' ===\n";

        while(true)
        {
            ++currentLine;

            syntaxCheck.reset();
            m_readerBuffer.Clear();
            results.Clear();

            prepareUserPrompt(outputStream, false, currentLine);

            try
            {
                readUserInput(results);

                while(!results.IsEmpty())
                {
                    auto pResult = results.PeekFront();
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

    void Interpreter::readUserInput(ezDeque<Ptr<LispObject>>& out_results)
    {
        auto pContMain = cont::createTopLevel(m_pState);
        auto pMainStack = cont::getStack(pContMain);

        auto pContRead = cont::create(pContMain, &reader::read);
        auto pReadStack = cont::getStack(pContRead);

        auto inputBuffer = std::string();
        auto pStream = stream::create(ezStringIterator());
        auto& syntaxCheck = m_pState->getReaderState()->m_syntaxCheckResult;

        // Multiline input, e.g. (define (fac n)
        //                         (if (<= n 1)
        //                           1
        //                           (* n (fac (- n 1)))))
        while(true)
        {
            try
            {
                std::getline(m_in, inputBuffer);
                m_readerBuffer.AppendFormat("%s", inputBuffer.c_str());
                stream::setIterator(pStream, m_readerBuffer.GetIteratorFront());

                // Multiple objects per line, e.g.: "(fac 1)  2  (isPrime 42)"
                while(true)
                {
                    pMainStack->clear();
                    pReadStack->clear();

                    pReadStack->push(pStream);

                    syntaxCheck.reset();

                    cont::setFunction(pContRead, &reader::read);
                    cont::trampoline(pContRead);

                    if(syntaxCheck.m_isPureWhitespace)
                    {
                        break;
                    }

                    auto pResult = pMainStack->get(0);
                    out_results.PushBack(pResult);

                    {
                        auto& iterator = stream::getIterator(pStream);
                        m_readerBuffer = iterator;
                        stream::setIterator(pStream, m_readerBuffer.GetIteratorFront());
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
                addPadding(*m_pState->getPrinterState()->m_pOutStream, '.');
                continue;
            }
            // If we reach this point, no exception was thrown and everything is fine.
            break;
        }
    }

    Ptr<LispObject> Interpreter::evaluateReaderOutput(Ptr<LispObject> pObject)
    {
        auto pContMain = cont::createTopLevel(m_pState);
        auto pMainStack = cont::getStack(pContMain);

        auto pContEvaluate = cont::create(pContMain, &eval::evaluate);
        cont::getStack(pContEvaluate)->push(m_pState->getGlobalEnvironment());
        cont::getStack(pContEvaluate)->push(pObject);

        cont::trampoline(pContEvaluate);

        return pMainStack->get(0);
    }

    void Interpreter::print(Ptr<LispObject> pToPrint)
    {
        auto pContMain = cont::createTopLevel(m_pState);

        auto pContRead = cont::create(pContMain, &printer::print);
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
        for(auto count = ezUInt32(0); count < maxCount; ++count)
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
        auto output = ezStringBuilder();
        lineBreak(output);
        outputStream << output;
    }

}
