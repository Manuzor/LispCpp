#include "stdafx.h"
#include "lcpp/cli/interpreter.h"
#include <iostream>

lcpp::Interpreter::Interpreter(const CInfo& cinfo) :
    m_pReader(cinfo.pReader),
    m_pEvaluator(cinfo.pEvaluator),
    m_pPrinter(cinfo.pPrinter),
    m_out(std::cout),
    m_in(std::cin)
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

        m_pPrinter->print(*pResult);
    }
}
