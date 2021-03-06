#include "stdafx.h"
#include "lcpp/cli/interpreter.h"
#include "lcpp/cli/ioUtils.h"
#include <iostream>
#include <string>
#include "lcpp/exceptions/exceptions.h"
#include "lcpp/core/typeSystem/types/nil.h"

lcpp::Interpreter::Interpreter(const CInfo& cinfo) :
    m_pPrinter(cinfo.pPrinter),
    m_out(std::cout),
    m_in(std::cin),
    m_szDataDir(),
    m_szUserDir()
{
    EZ_ASSERT(m_pPrinter, "Invalid printer pointer!");
}

lcpp::Interpreter::~Interpreter()
{
}

void lcpp::Interpreter::initialize()
{
    ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);

    ezStringBuilder workingDir;
    getCurrentWorkingDirectory(workingDir);

    m_szDataDir.Clear();
    m_szDataDir.AppendPath(workingDir.GetData(), "data0");
    m_szDataDir.MakeCleanPath();

    // Data dir
    //////////////////////////////////////////////////////////////////////////
    {
        auto result = ezFileSystem::AddDataDirectory(m_szDataDir.GetData(), ezFileSystem::ReadOnly, "data");

        if(result.Failed())
        {
            ezStringBuilder message(m_szDataDir);
            message.Prepend("Unable add data dir: ");
            throw std::exception(message.GetData());
        }
    }

    // User dir
    //////////////////////////////////////////////////////////////////////////
    {
        m_szUserDir = m_szDataDir;
        m_szUserDir.AppendPath("user");
        auto result = ezFileSystem::AddDataDirectory(m_szUserDir.GetData(), ezFileSystem::ReadOnly, "user-data");

        if(result.Failed())
        {
            ezStringBuilder message(m_szUserDir);
            message.Prepend("Unable add user dir: ");
            throw std::exception(message.GetData());
        }
    }
}

void
lcpp::Interpreter::shutdown()
{
}

void lcpp::Interpreter::loadBase()
{
    auto pReader = LispRuntime::instance()->reader();
    auto pEvaluator = LispRuntime::instance()->evaluator();

    ezStringBuilder stdlibDir(m_szDataDir);
    stdlibDir.AppendPath("base", "stdlib.lisp");
    stdlibDir.MakeCleanPath();

    ezFileReader file_stdlib;
    if(file_stdlib.Open(stdlibDir.GetData(), ezFileMode::Read).Failed())
    {
        auto message = ezStringBuilder();
        message.AppendFormat("Unable to load \"%s\"!", stdlibDir);
        throw std::exception(message.GetData());
    }
    auto size = file_stdlib.GetFileSize();
    auto bufferSize = ezUInt32(size)
                    + 1; // null terminator

    auto buffer = EZ_NEW_RAW_BUFFER(LispRuntime::instance()->allocator().get(), char, bufferSize);
    LCPP_SCOPE_EXIT{ EZ_DELETE_RAW_BUFFER(LispRuntime::instance()->allocator().get(), buffer); };

    // null terminator
    buffer[bufferSize - 1] = '\0';
    
    ezStringBuilder content(LispRuntime::instance()->allocator().get());
    content.Reserve(bufferSize);

    file_stdlib.ReadBytes(buffer, bufferSize - 1);

    content.Append("(begin ");
    content.Append(buffer);
    content.Append(')');
    auto contentIter = content.GetIteratorFront();
    Ptr<LispObject> pResult;

    pReader->syntaxCheckResult()->reset();

    pResult = pReader->read(contentIter, false);
    pResult = pEvaluator->evalulate(pResult);
    m_pPrinter->print(pResult);
}

ezInt32 lcpp::Interpreter::repl()
{
    std::ios_base::sync_with_stdio(false);

    auto pReader = LispRuntime::instance()->reader();
    auto pEvaluator = LispRuntime::instance()->evaluator();

    m_pPrinter->setOutputStream(m_out);

    Ptr<LispObject> pResult = LCPP_NIL;

    ezStringBuilder buffer;
    std::string inputBuffer("");

    auto prompt = ezStringBuilder();
    auto currentLine = ezUInt32(0);

    Reader::SyntaxCheckResult syntaxCheck;

    try
    {
        loadBase();
    }
    catch(exceptions::Assertion& assertionException)
    {
        auto message = ezStringBuilder("Assertion failed in stdlib");
        if(assertionException.what())
        {
            message.AppendFormat(": %s", assertionException.what());
        }
        else
        {
            message.Append('.');
        }

        message.Append('\n');

        m_pPrinter->print(message);
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
        ezTelemetry::PerFrameUpdate();

        ++currentLine;
        buffer.Clear();
        prompt.Format("%u> ", currentLine);
        m_pPrinter->print(prompt);

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

                ++currentLine;

                auto leftPadding = ezStringBuilder();
                for(auto i = prompt.GetCharacterCount(); i > 0; --i)
                {
                    leftPadding.Append(' ');
                }
                
                m_pPrinter->print(leftPadding);
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
            auto message = ezStringBuilder();
            auto& sourcePos = syntaxCheck.cursor.currentPosition();
            message.AppendFormat("StdIn(%u:%u): Input error: %s\n",
                                 currentLine,
                                 sourcePos.posInLine + 1,
                                 e.what());
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
        catch(exceptions::Assertion& assertionException)
        {
            auto message = ezStringBuilder("Assertion failed");
            if (assertionException.what())
            {
                message.AppendFormat(": %s", assertionException.what());
            }
            else
            {
                message.Append('.');
            }

            message.Append('\n');

            m_pPrinter->print(message);
            continue;
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
