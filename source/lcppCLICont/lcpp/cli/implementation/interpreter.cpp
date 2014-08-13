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

#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/exceptions/readerException.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/exceptions/noBindingFoundException.h"

lcpp::Interpreter::Interpreter() :
    m_state(),
    m_pState(&m_state),
    m_out(std::cout),
    m_in(std::cin),
    m_szDataDir()
{
}

lcpp::Interpreter::~Interpreter()
{
}

void lcpp::Interpreter::initialize()
{
    ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);

    auto dataDir = ezStringBuilder();
    auto applicationDir = getCurrentWorkingDirectory();
    dataDir.AppendPath(applicationDir.GetData(), "data");
    dataDir.MakeCleanPath();
    auto result = ezFileSystem::AddDataDirectory(dataDir.GetData(), ezFileSystem::ReadOnly, "data");

    if (!result.Succeeded())
    {
        dataDir.Prepend("Unable to add data dir: ");
        throw std::exception(dataDir.GetData());
    }
    m_szDataDir = dataDir;

    m_pState->initialize();
}

void
lcpp::Interpreter::shutdown()
{
    m_pState->shutdown();
}

ezInt32 lcpp::Interpreter::repl()
{
    std::ios_base::sync_with_stdio(false);

    auto buffer = ezStringBuilder();
    auto inputBuffer = std::string("");

    auto pReadStream = stream::create(buffer.GetIteratorFront());
    auto pNewLine = str::create("\n");
    auto pResult = LCPP_pNil;

    auto prompt = ezStringBuilder();
    auto currentLine = ezUInt32(0);

    auto& syntaxCheck = m_pState->getReaderState()->m_syntaxCheckResult;
    auto& outputStream = *m_pState->getPrinterState()->m_pOutStream;

    outputStream << "=== Scheme interpreter 'lcpp' ===";

    auto printNewLine = true;

    while(true)
    {
        ++currentLine;

        if(printNewLine)
        {
            outputStream << "\n";
        }
        else
        {
            printNewLine = true;
        }

        prompt.Format("%u> ", currentLine);
        outputStream << prompt;

        std::getline(m_in, inputBuffer);
        buffer.Format("%s", inputBuffer.c_str());

        stream::setIterator(pReadStream, buffer.GetIteratorFront());

        try
        {
            // Read
            syntaxCheck.reset();
            pResult = readStream(pReadStream);

            // Eval
            pResult = evalGlobally(pResult);
            printNewLine = !isVoid(pResult);

            // Print
            if(!isVoid(pResult))
            {
                print(pResult);
            }
        }
        catch(exceptions::Reader& ex)
        {
            const auto& sourcePos = syntaxCheck.m_cursor.getPosition();

            auto info = ezStringBuilder();

            for(auto i = prompt.GetCharacterCount() + sourcePos.m_column; i > 0; --i)
            {
                info.Append('-');
            }
            info.Append("^\n");

            info.AppendFormat("Parsing error in stdin(%u:%u): %s", sourcePos.m_line + currentLine, sourcePos.m_column, ex.what());

            outputStream << info.GetData();
        }
        catch(exceptions::NoBindingFound& ex)
        {
            outputStream << ex.what();
        }
        catch(exceptions::ExceptionBase& ex)
        {
            outputStream << "Error: " << ex.what();
        }
        catch(std::exception& ex)
        {
            outputStream << "Fatal error: " << ex.what();
        }
        catch (...)
        {
            outputStream << "Unknown error occurred.";
        }
    }
}

lcpp::Ptr<lcpp::LispObject> lcpp::Interpreter::readStream(Ptr<LispObject> pStream)
{
    auto pContMain = cont::createTopLevel(m_pState);
    auto pMainStack = cont::getStack(pContMain);

    auto pContRead = cont::create(pContMain, &reader::read);
    cont::getStack(pContRead)->push(pStream);

    cont::trampoline(pContRead);

    return pMainStack->get(0);
}

lcpp::Ptr<lcpp::LispObject> lcpp::Interpreter::evalGlobally(Ptr<LispObject> pToEval)
{
    auto pContMain = cont::createTopLevel(m_pState);
    auto pMainStack = cont::getStack(pContMain);

    auto pContRead = cont::create(pContMain, &eval::evaluate);
    cont::getStack(pContRead)->push(m_pState->getGlobalEnvironment());
    cont::getStack(pContRead)->push(pToEval);

    cont::trampoline(pContRead);

    return pMainStack->get(0);
}

lcpp::Ptr<lcpp::LispObject> lcpp::Interpreter::print(Ptr<LispObject> pToPrint)
{
    auto pContMain = cont::createTopLevel(m_pState);
    auto pMainStack = cont::getStack(pContMain);

    auto pContRead = cont::create(pContMain, &printer::print);
    cont::getStack(pContRead)->push(pToPrint);

    cont::trampoline(pContRead);

    return pMainStack->get(0);
}
