#pragma once
#include "lcpp/core/sourceCursor.h"

namespace lcpp
{
    class SyntaxCheckResult
    {
    public:

        bool m_valid;

        bool m_hasParenthesis;

        bool m_isPureWhitespace;

        ezInt32 m_parenthesisBalance;

        SourceCursor m_cursor;

        String m_info;

    public:
        
        SyntaxCheckResult();

        bool isComplete() const;

        void reset();
    };
}

#include "lcpp/core/impl/syntaxCheckResult.inl"
