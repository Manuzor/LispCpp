#pragma once

namespace lcpp
{
    namespace reader
    {

        EZ_FORCE_INLINE
        State::State() :
            m_separators(" \t\r\n\v\f\a"),
            m_symbolDelimiters("()"),
            m_commentDelimiter(';'),
            m_newLineDelimiter('\n'),
            m_syntaxCheckResult()
        {
        }
    }
}
