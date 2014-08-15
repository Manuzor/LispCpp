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

    private:
        LispRuntimeState m_state;
        Ptr<LispRuntimeState> m_pState;

        std::ostream& m_out;
        std::istream& m_in;

        ezStringBuilder m_szDataDir;

        ezStringBuilder m_userPrompt;

    private:

        void prepareUserPrompt(ezStreamWriterBase& outputStream, bool printNewLine, ezUInt32 currentLine);

        Ptr<LispObject> evaluateStream(Ptr<LispObject> pStream);
        void print(Ptr<LispObject> pToPrint);
    };
    
}
