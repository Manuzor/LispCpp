#pragma once

namespace lcpp
{
    struct SourcePosition
    {
        ezUInt32 line;        ///< 0-based
        ezUInt32 posInLine;   ///< 0-based
        ezUInt32 streamIndex; ///< 0-based

        SourcePosition();
    };

    struct SourceCursor
    {
    public:
        SourceCursor();

        void reset();

        void lineBreak();

        void advance();

        void operator ++();

        SourcePosition& currentPosition();

        const SourcePosition& currentPosition() const;

    private:
        SourcePosition position;
    };
}

#include "lcpp/core/implementation/sourceCursor.inl"
