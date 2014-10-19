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

        ezStringBuilder m_userPrompt;
        ezStringBuilder m_readerBuffer;

    private:

        void evalInitFile();

        void prepareUserPrompt(ezStreamWriterBase& outputStream, bool printNewLine, ezUInt32 currentLine);

        void readUserInput(ezDeque<Ptr<LispObject>>& out_results);
        Ptr<LispObject> evaluateReaderOutput(StackPtr<LispObject> pObject);
        void print(StackPtr<LispObject> pToPrint);

        void addPadding(ezStringBuilder& builder, ezUInt32 paddingCharacter = ' ');
        void addPadding(ezStreamWriterBase& outputStream, ezUInt32 paddingCharacter = ' ');
        void lineBreak(ezStringBuilder& builder);
        void lineBreak(ezStreamWriterBase& outputStream);
    };
}
