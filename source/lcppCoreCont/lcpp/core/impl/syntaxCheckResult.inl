
namespace lcpp
{
    EZ_FORCE_INLINE
    SyntaxCheckResult::SyntaxCheckResult()
    {
        reset();
    }

    EZ_FORCE_INLINE
    bool SyntaxCheckResult::isComplete() const
    {
        return m_parenthesisBalance == 0;
    }

    EZ_FORCE_INLINE
    void SyntaxCheckResult::reset()
    {
        m_valid = false;
        m_hasParenthesis = false;
        m_isPureWhitespace = true;
        m_parenthesisBalance = 0;
        m_cursor.reset();
        m_info.Clear();
    }
}
