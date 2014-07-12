#pragma once

namespace lcpp
{
    class Interpreter
    {
    public:

        struct CInfo
        {
            Ptr<LispRuntime> pRuntime;
            Ptr<Printer> pPrinter;
        };

    public:
        Interpreter(const CInfo& cinfo);
        ~Interpreter();

        void initialize();
        void shutdown();

        void loadBase();

        ezInt32 repl();

    private:
        Ptr<LispRuntime> m_pRuntime;
        Ptr<Printer> m_pPrinter;

        std::ostream& m_out;
        std::istream& m_in;

        ezStringBuilder m_szDataDir;
        ezStringBuilder m_szBaseDir;
    };
    
}
