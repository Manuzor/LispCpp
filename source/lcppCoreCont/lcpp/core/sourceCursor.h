#pragma once

namespace lcpp
{
    struct SourcePosition
    {
        ezUInt32 m_line;        ///< 0-based
        ezUInt32 m_column;      ///< 0-based
        ezUInt32 m_streamIndex; ///< 0-based

        SourcePosition();
    };

    struct SourceCursor
    {
    public:
        SourceCursor();

        void reset(); // [tested]

        void lineBreak(); // [tested]

        void advance(); // [tested]

        SourcePosition& getPosition(); // [tested]

        const SourcePosition& getPosition() const; // [tested]

    private:

        SourcePosition m_position;

    };
}

#include "lcpp/core/impl/sourceCursor.inl"
