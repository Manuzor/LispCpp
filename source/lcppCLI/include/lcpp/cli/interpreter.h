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

        void run();

    private:
        Reader* m_pReader;
        IEvaluator* m_pEvaluator;
        Printer* m_pPrinter;
    };
    
}
