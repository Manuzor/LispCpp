#pragma

namespace lcpp
{
    struct SourcePosition
    {
        ezUInt32 line;
        ezUInt32 posInLine;
        ezUInt32 streamIndex;

        SourcePosition() :
            line(0),
            posInLine(0),
            streamIndex(0)
        {
        }
    };

    struct SourceCursor
    {
    public:
        inline SourceCursor() :
            position()
        {
        }

        inline void reset()
        {
            position = SourcePosition();
        }

        inline void lineBreak()
        {
            position.line++;
            position.posInLine = ezUInt32(-1);// will be corrected with the next call of advance()
            position.streamIndex++;
        }

        inline void advance()
        {
            position.posInLine++;
            position.streamIndex++;
        }

        inline void operator ++()
        {
            advance();
        }

        inline SourcePosition& currentPosition()
        {
            return position;
        }

        inline const SourcePosition& currentPosition() const
        {
            return position;
        }

    private:
        SourcePosition position;
    };
}
