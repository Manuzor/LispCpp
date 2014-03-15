#pragma once

#include "lcpp/gui/fontInfo.h"
#include "lcpp/gui/text.h"
#include "lcpp/gui/cursor.h"

namespace lcpp
{
    class Console : public sf::Drawable
    {
    public:
        struct CInfo
        {
            FontInfo mainFontInfo;
            sf::FloatRect bounds;

            CInfo() :
                mainFontInfo(),
                bounds()
            {
            }
        };

    private:
        sf::Font m_mainFont;
        FontInfo m_mainFontInfo;

        Text m_input;

        sf::FloatRect m_bounds;

        Cursor m_cursor;

    public:
        Console(const CInfo& cinfo) :
            m_mainFontInfo(cinfo.mainFontInfo),
            m_bounds(cinfo.bounds)
        {}
        virtual ~Console() {}

        bool initialize();

        void shutdown();

        void update(sf::Time elapsedTime);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        //inline Text& text(){ return m_textData; }
        inline Text& inputText(){ return m_input; }
        //inline Text& infoText(){ return m_infoData; }

    private:

        //void buildText();
        //void buildInputText();
        //void buildInfoText();
    };
}
