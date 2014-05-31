#pragma once

namespace lcpp
{
    class Interpreter
    {
    public:

        struct CInfo
        {
            Reader* pReader;
            IEvaluator* pEvaluator;
            Printer* pPrinter;

            CInfo() :
                pReader(nullptr),
                pEvaluator(nullptr),
                pPrinter(nullptr)
            {
            }
        };

    public:
        Interpreter(const CInfo& cinfo);
        ~Interpreter();

        ezInt32 repl();

    private:
        Ptr<Reader> m_pReader;
        Ptr<IEvaluator> m_pEvaluator;
        Ptr<Printer> m_pPrinter;

        std::ostream& m_out;
        std::istream& m_in;
    };
    
}
