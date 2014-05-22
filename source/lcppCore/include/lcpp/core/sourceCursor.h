#pragma

namespace lcpp
{
    struct SourceCursor
    {
        ezUInt64 line;
        ezUInt64 posInLine;
        ezUInt64 streamIndex;

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
            posInLine = 0;// will be correct with the next call of advance()
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
