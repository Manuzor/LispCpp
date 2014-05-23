#include "stdafx.h"
#include "lcpp/cli/interpreter.h"
#include <iostream>

lcpp::Interpreter::Interpreter(const CInfo& cinfo) :
    m_pReader(cinfo.pReader),
    m_pEvaluator(cinfo.pEvaluator),
    m_pPrinter(cinfo.pPrinter)
{
    // TODO assert pointers are valid
}

lcpp::Interpreter::~Interpreter()
{
    m_pReader = nullptr;
    m_pEvaluator = nullptr;
    m_pPrinter = nullptr;
}

void lcpp::Interpreter::run()
{
    std::ios_base::sync_with_stdio(false);

    std::ostream& out = std::cout;
    std::istream& in = std::cin;

    SchemeObject* pResult = &SCHEME_NIL;

    ezStringBuilder buffer;
    std::string inputBuffer("");

    m_pPrinter->print("=== Scheme interpreter 'lcpp' ===\n");

    while(true)
    {
        buffer.Clear();
        m_pPrinter->print("> ");

        try
        {
            bool abort = false;
            do
            {
                std::getline(in, inputBuffer);
                buffer.Append(inputBuffer.c_str());
                buffer.Append('\n');

                auto syntaxCheck = m_pReader->checkBasicSyntax(buffer.GetIteratorFront());

                if (!syntaxCheck.valid)
                {
                    throw exceptions::InvalidSyntax("Invalid syntax!");
                }

                abort = syntaxCheck.isComplete()
                    && (syntaxCheck.hasParenthesis || syntaxCheck.parenthesisBalance <= 0);
            } while (!abort);
            pResult = &m_pReader->read(buffer.GetIteratorFront());
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
            // TODO eval
            pResult = &m_pEvaluator->evalulate(*pResult);
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
