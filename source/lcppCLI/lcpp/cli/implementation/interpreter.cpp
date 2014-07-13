#include "stdafx.h"
#include "lcpp/cli/interpreter.h"
#include "lcpp/cli/ioUtils.h"
#include <iostream>
#include <string>

lcpp::Interpreter::Interpreter(const CInfo& cinfo) :
    m_pRuntime(cinfo.pRuntime),
    m_pPrinter(cinfo.pPrinter),
    m_out(std::cout),
    m_in(std::cin),
    m_szDataDir()
{
    EZ_ASSERT(m_pRuntime, "Invalid runtime pointer!");
    EZ_ASSERT(m_pPrinter, "Invalid printer pointer!");
}

lcpp::Interpreter::~Interpreter()
{
}

void lcpp::Interpreter::initialize()
{
    m_pRuntime->initialize();

    ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);

    ezStringBuilder dataDir;
    dataDir.AppendPath(ezOSFile::GetApplicationDirectory(), "../../data");
    dataDir.MakeCleanPath();
    auto result = ezFileSystem::AddDataDirectory(dataDir.GetData(), ezFileSystem::ReadOnly, "data-root");

    if (!result.IsSuccess())
    {
        dataDir.Prepend("Unable add data dir: ");
        throw std::exception(dataDir.GetData());
    }
    m_szDataDir = dataDir;

    dataDir.AppendPath("base");

    result = ezFileSystem::AddDataDirectory(dataDir.GetData(), ezFileSystem::ReadOnly, "data/base");

    if(!result.IsSuccess())
    {
        dataDir.Prepend("Unable add base dir: ");
        throw std::exception(dataDir.GetData());
    }

    m_szBaseDir = dataDir;
}

void
lcpp::Interpreter::shutdown()
{
    m_pRuntime->shutdown();
}

void lcpp::Interpreter::loadBase()
{
    auto pReader = m_pRuntime->reader();
    auto pEvaluator = m_pRuntime->evaluator();

    ezFileReader file_stdlib;
    if(file_stdlib.Open("stdlib.lisp", ezFileMode::Read) == EZ_FAILURE)
    {
        throw std::exception("Unable to load stdlib.lisp!");
    }
    auto size = file_stdlib.GetFileSize();
    auto bufferSize = ezUInt32(size) + 1;

    auto buffer = LCPP_NEW_RAW_BUFFER(m_pRuntime->allocator().get(), char, bufferSize);
    LCPP_SCOPE_EXIT{ LCPP_DELETE_RAW_BUFFER(m_pRuntime->allocator().get(), buffer); };

    // null terminator
    buffer[bufferSize - 1] = '\0';
    
    ezStringBuilder content(m_pRuntime->allocator().get());
    content.Reserve(bufferSize);

    file_stdlib.ReadBytes(buffer, bufferSize - 1);

    content.Append(buffer);
    auto contentIter = content.GetIteratorFront();
    Ptr<LispObject> pResult;

    pReader->syntaxCheckResult()->reset();

    while(true)
    {
        pReader->skipSeparators(contentIter);
        if(!contentIter.IsValid()) { break; }
        
        pResult = pReader->read(contentIter, false);
        pResult = pEvaluator->evalulate(pResult);
        m_pPrinter->print(pResult);
    }
}

ezInt32 lcpp::Interpreter::repl()
{
    std::ios_base::sync_with_stdio(false);

    auto pReader = m_pRuntime->reader();
    auto pEvaluator = m_pRuntime->evaluator();

    m_pPrinter->setOutputStream(m_out);

    Ptr<LispObject> pResult = LCPP_NIL;

    ezStringBuilder buffer;
    std::string inputBuffer("");

    Reader::SyntaxCheckResult syntaxCheck;

    try
    {
        loadBase();
    }
    catch(exceptions::ExceptionBase& e)
    {
        ezStringBuilder message;
        message.AppendFormat("Error loading stdlib: %s\n", e.what());
        m_pPrinter->print(message);
    }

    m_pPrinter->print("=== Scheme interpreter 'lcpp' ===\n");

    while(true)
    {
        buffer.Clear();
        m_pPrinter->print("> ");

        try
        {
            while(true)
            {
                std::getline(m_in, inputBuffer);
                buffer.Append(inputBuffer.c_str());
                buffer.Append('\n');

                syntaxCheck = pReader->checkBasicSyntax(buffer.GetIteratorFront());

                if(syntaxCheck.isPureWhitespace
                   || syntaxCheck.isComplete() && syntaxCheck.valid
                   || syntaxCheck.parenthesisBalance < 0)
                {
                    break;
                }
            }

            if(syntaxCheck.isPureWhitespace) { continue; }

            if(!syntaxCheck.valid)
            {
                throw exceptions::InvalidSyntax("Invalid syntax!");
            }

            pResult = pReader->read(buffer.GetIteratorFront());
        }
        catch (exceptions::ExceptionBase& e)
        {
            ezStringBuilder message;
            message.AppendFormat("Input error: %s\n", e.what());
            m_pPrinter->print(message);
            continue;
        }

        try
        {
            pResult = pEvaluator->evalulate(pResult);
        }
        catch(exceptions::Exit& e)
        {
            return e.m_status;
        }
        catch(exceptions::ExceptionBase& e)
        {
            ezStringBuilder message;
            message.AppendFormat("Eval error: %s\n", e.what());
            m_pPrinter->print(message);
            continue;
        }

        m_pPrinter->print(pResult);
    }
}
