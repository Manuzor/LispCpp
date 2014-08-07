#pragma once
#include "lcpp/core/runtime.h"

namespace lcpp
{
    class Interpreter
    {
    public:
        Interpreter();
        ~Interpreter();

        void initialize();
        void shutdown();

        ezInt32 repl();

    private:
        LispRuntimeState m_state;

        std::ostream& m_out;
        std::istream& m_in;

        ezStringBuilder m_szDataDir;
    };
    
}
