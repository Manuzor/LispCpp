#include "stdafx.h"
#include "lcpp/cli/interpreter.h"
#include "lcpp/cli/ioUtils.h"
#include <iostream>

lcpp::Interpreter::Interpreter(const CInfo& cinfo) :
    m_pReader(cinfo.pReader),
    m_pEvaluator(cinfo.pEvaluator),
    m_pPrinter(cinfo.pPrinter),
    m_out(std::cout),
    m_in(std::cin),
    m_szDataDir()
{
    EZ_ASSERT(m_pReader, "Invalid reader pointer!");
    EZ_ASSERT(m_pEvaluator, "Invalid evaluator pointer!");
    EZ_ASSERT(m_pPrinter, "Invalid printer pointer!");
}

lcpp::Interpreter::~Interpreter()
{
}

void lcpp::Interpreter::initialize()
{
    m_pReader->initialize();
    m_pEvaluator->initialize();

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

void lcpp::Interpreter::loadBase()
{
    ezFileReader file_stdlib;
    if(file_stdlib.Open("stdlib.lisp", ezFileMode::Read) == EZ_FAILURE)
    {
        throw std::exception("Unable to load stdlib.lisp!");
    }
    auto size = file_stdlib.GetFileSize();
    auto bufferSize = ezUInt32(size) + 1;

    auto buffer = new char[bufferSize];
    LCPP_SCOPE_EXIT{ delete[] buffer; };

    // null terminator
    buffer[bufferSize - 1] = '\0';
    
    ezStringBuilder content;
    content.Reserve(bufferSize);

    file_stdlib.ReadBytes(buffer, bufferSize - 1);

    content.Append(buffer);
    auto contentIter = content.GetIteratorFront();
    Ptr<SchemeObject> pResult;

    m_pReader->syntaxCheckResult()->reset();

    while(true)
    {
        m_pReader->skipSeparators(contentIter);
        if(!contentIter.IsValid()) { break; }
        
        pResult = m_pReader->read(contentIter, false);
        pResult = m_pEvaluator->evalulate(pResult);
        m_pPrinter->print(pResult);
    }
}

void lcpp::Interpreter::runUnittests()
{

}

ezInt32 lcpp::Interpreter::repl()
{
    std::ios_base::sync_with_stdio(false);

    m_pPrinter->setOutputStream(m_out);

    Ptr<SchemeObject> pResult = SCHEME_NIL_PTR;

    ezStringBuilder buffer;
    std::string inputBuffer("");

    Reader::SyntaxCheckResult syntaxCheck;

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

                syntaxCheck = m_pReader->checkBasicSyntax(buffer.GetIteratorFront());

                if(syntaxCheck.isPureWhitespace
                   || syntaxCheck.isComplete()
                   && (syntaxCheck.hasParenthesis
                       || syntaxCheck.parenthesisBalance <= 0))
                {
                    break;
                }
            }

            if(syntaxCheck.isPureWhitespace) { continue; }

            if(!syntaxCheck.valid)
            {
                throw exceptions::InvalidSyntax("Invalid syntax!");
            }

            pResult = m_pReader->read(buffer.GetIteratorFront());
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
            pResult = m_pEvaluator->evalulate(pResult);
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
