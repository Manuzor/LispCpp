#pragma once

namespace lcpp
{

    class Text
    {
        bool m_isModified;
        ezStringBuilder m_builder;

        float m_maxWidth;
    public:
        inline Text() : m_isModified(false), m_maxWidth(0.0f), m_builder() {}
        inline ~Text() { m_builder.Clear(); }

        void append(ezUInt32 unicodeChar);
        void append(const char* cstring);
        void append(const ezString& str);
        void appendFormat(const char* format, ...);

        void prepend(ezUInt32 unicodeChar);
        void prepend(const char* cstring);
        void prepend(const ezString& str);
        void prependFormat(const char* format, ...);

        void shrink(ezUInt32 shrinkCharsFront, ezUInt32 shrinkCharsBack);

        inline void clear(){ m_builder.Clear(); }

        inline const char* data(){ return m_builder.GetData(); }

        inline void markModified(){ m_isModified = true; }
        inline void unmarkModified(){ m_isModified = false; }
        inline bool isModified(){ return m_isModified; }

        inline void maxWidth(float width) { m_maxWidth = width; }
        inline float maxWidth() const { return m_maxWidth; }
    };

}