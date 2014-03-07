#pragma once

#include "lcpp/gui/fontInfo.h"

namespace lcpp
{
    class Console : public sf::Drawable
    {
    public:
        struct CInfo
        {
            FontInfo mainFontInfo;

            CInfo() :
                mainFontInfo()
            {
            }
        };

    private:
        sf::Font m_mainFont;
        FontInfo m_mainFontInfo;

        ezDynamicArray<sf::Text> m_lines;
        sf::Text m_info;

    public:
        Console() {}
        virtual ~Console(){}

        bool initialize(const CInfo& cinfo);

        void shutdown();

        void update(sf::Time elapsedTime);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    private:

        void buildText();
        ezUInt32 calcNumLines();
        void setupLines();
    };
}
