#pragma once
#include "lcpp/core/runtime.h"

namespace lcpp
{
    class LispObject;

    class Interpreter
    {
    public:
        Interpreter();
        ~Interpreter();

        void initialize();
        void shutdown();

        ezInt32 repl();

        Ptr<LispObject> readStream(Ptr<LispObject> pStream);

        Ptr<LispObject> evalGlobally(Ptr<LispObject> pToEval);

        Ptr<LispObject> print(Ptr<LispObject> pToPrint);

    private:
        LispRuntimeState m_state;
        Ptr<LispRuntimeState> m_pState;

        std::ostream& m_out;
        std::istream& m_in;

        ezStringBuilder m_szDataDir;
    };
    
}
