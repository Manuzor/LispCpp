#pragma once

#include "lcpp/gui/fontInfo.h"

namespace lcpp
{
    class Console : public sf::Drawable
    {
        struct TextData
        {

        };
    public:
        struct CInfo
        {
            FontInfo mainFontInfo;

            CInfo() :
                mainFontInfo()
            {
            }
        };

        class Text
        {
            bool m_isModified;
            ezStringBuilder m_builder;
        public:
            inline Text() : m_isModified(false), m_builder() {}
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
        };

    private:
        sf::Font m_mainFont;
        FontInfo m_mainFontInfo;

        Console::Text m_textData;
        Console::Text m_inputData;
        Console::Text m_infoData;

        ezDynamicArray<sf::Text> m_textLines;
        sf::Text m_input;
        sf::Text m_info;

    public:
        Console(const CInfo& cinfo) :
            m_mainFontInfo(cinfo.mainFontInfo)
        {}
        virtual ~Console() {}

        bool initialize();

        void shutdown();

        void update(sf::Time elapsedTime);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        inline Text& text(){ return m_textData; }
        inline Text& inputText(){ return m_inputData; }
        inline Text& infoText(){ return m_infoData; }

    private:

        void buildText();
        void buildInputText();
        void buildInfoText();

        ezUInt32 calcNumLines();
        void setupLines();
    };
}
