
namespace lcpp
{
    EZ_FORCE_INLINE
    SourcePosition::SourcePosition() :
        m_line(0),
        m_column(0),
        m_streamIndex(0)
    {
    }

    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    SourceCursor::SourceCursor() :
        m_position()
    {
    }

    EZ_FORCE_INLINE
    void SourceCursor::reset()
    {
        m_position = SourcePosition();
    }

    EZ_FORCE_INLINE
    void SourceCursor::lineBreak()
    {
        m_position.m_line++;
        m_position.m_column = 0;
        m_position.m_streamIndex++;
    }

    EZ_FORCE_INLINE
    void SourceCursor::advance()
    {
        m_position.m_column++;
        m_position.m_streamIndex++;
    }

    EZ_FORCE_INLINE
    SourcePosition& SourceCursor::getPosition()
    {
        return m_position;
    }

    EZ_FORCE_INLINE
    const SourcePosition& SourceCursor::getPosition() const
    {
        return m_position;
    }

}
