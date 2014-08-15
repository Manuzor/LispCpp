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
        m_userPrompt()
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

        auto pReadStream = stream::create(ezStringIterator());
        auto pResult = LCPP_pNil;

        auto readerBuffer = ezStringBuilder();
        auto inputBuffer = std::string("");

        auto currentLine = ezUInt32(0);

        auto& syntaxCheck = m_pState->getReaderState()->m_syntaxCheckResult;
        auto& outputStream = *m_pState->getPrinterState()->m_pOutStream;

        outputStream << "=== Scheme interpreter 'lcpp' ===";

        auto printNewLine = true;

        while(true)
        {
            ++currentLine;

            syntaxCheck.reset();
            readerBuffer.Clear();

            prepareUserPrompt(outputStream, printNewLine, currentLine);

            printNewLine = true;

            try
            {
                while(true)
                {
                    std::getline(m_in, inputBuffer);
                    readerBuffer.AppendFormat("%s", inputBuffer.c_str());

                    stream::setIterator(pReadStream, readerBuffer.GetIteratorFront());

                    try
                    {
                        pResult = evaluateStream(pReadStream);

                        // Print
                        if(!isVoid(pResult))
                        {
                            printNewLine = true;
                            print(pResult);
                        }
                        else
                        {
                            printNewLine = false;
                        }

                        break;
                    }
                    catch(exceptions::MissingRightListDelimiter&)
                    {
                        readerBuffer.Append('\n');
                    }
                }
            }
            catch(exceptions::ReaderBase& ex)
            {
                const auto& sourcePos = syntaxCheck.m_cursor.getPosition();

                auto info = ezStringBuilder();

                for(auto i = m_userPrompt.GetCharacterCount() + sourcePos.m_column; i > 0; --i)
                {
                    info.Append('-');
                }
                info.Append("^\n");

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
                outputStream << "Unexpected error: " << ex.what();
            }
            catch(std::exception& ex)
            {
                outputStream << "Fatal error: " << ex.what();
            }
            catch(...)
            {
                outputStream << "Unknown error occurred.";
            }
        }
    }

    Ptr<LispObject> Interpreter::evaluateStream(Ptr<LispObject> pStream)
    {
        auto pContMain = cont::createTopLevel(m_pState);
        auto pMainStack = cont::getStack(pContMain);

        auto pContEvaluate = cont::create(pContMain, &eval::evaluate);
        cont::getStack(pContEvaluate)->push(m_pState->getGlobalEnvironment());

        auto pContRead = cont::create(pContEvaluate, &reader::read);
        cont::getStack(pContRead)->push(pStream);

        cont::trampoline(pContRead);

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
            outputStream << "\n";
        }

        m_userPrompt.Format("%u> ", currentLine);
        outputStream << m_userPrompt;
    }

}
