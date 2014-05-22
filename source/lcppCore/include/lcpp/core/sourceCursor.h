#pragma

namespace lcpp
{
    struct SourceCursor
    {
        ezUInt32 line;
        ezUInt32 posInLine;
        ezUInt32 streamIndex;

        inline static SourceCursor beginning()
        {
            SourceCursor cursor;
            cursor.line = 1;
            cursor.posInLine = 1;
            cursor.streamIndex = 1;
            return cursor;
        }

        inline SourceCursor() :
            line(0),
            posInLine(0),
            streamIndex(0)
        {
        }

        inline void lineBreak()
        {
            line++;
            posInLine = ezUInt32(- 1);// will be corrected with the next call of advance()
            streamIndex++;
        }

        inline void advance()
        {
            posInLine++;
            streamIndex++;
        }

        inline void operator ++()
        {
            advance();
        }
    };
}
