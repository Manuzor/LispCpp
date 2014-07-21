
inline
lcpp::SourcePosition::SourcePosition() :
    line(0),
    posInLine(0),
    streamIndex(0)
{
}

//////////////////////////////////////////////////////////////////////////

inline
lcpp::SourceCursor::SourceCursor() :
    position()
{
}

inline
void
lcpp::SourceCursor::reset()
{
    position = SourcePosition();
}

inline
void
lcpp::SourceCursor::lineBreak()
{
    position.line++;
    position.posInLine = ezUInt32(-1);// will be corrected with the next call of advance()
    position.streamIndex++;
}

inline
void
lcpp::SourceCursor::advance()
{
    position.posInLine++;
    position.streamIndex++;
}

inline
void
lcpp::SourceCursor::operator ++()
{
    advance();
}

inline
lcpp::SourcePosition&
lcpp::SourceCursor::currentPosition()
{
    return position;
}

inline
const lcpp::SourcePosition&
lcpp::SourceCursor::currentPosition() const
{
    return position;
}
