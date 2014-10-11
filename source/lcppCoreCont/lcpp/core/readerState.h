#pragma once
#include "lcpp/core/syntaxCheckResult.h"

namespace lcpp
{
    class LispObject;

    namespace reader
    {
        class State
        {
        public:

            ezString m_separators;
            ezString m_symbolDelimiters;
            ezUInt32 m_commentDelimiter;
            ezUInt32 m_newLineDelimiter;
            SyntaxCheckResult m_syntaxCheckResult;
            Ptr<LispObject> m_pMacroEnv;

            State();
        };
    }
}

#include "lcpp/core/impl/readerState.inl"
