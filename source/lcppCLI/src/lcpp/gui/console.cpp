#include "stdafx.h"
#include "lcpp/gui/console.h"


bool lcpp::Console::initialize( const CInfo& cinfo )
{
    m_mainFontInfo = cinfo.mainFontInfo;

    if (!m_mainFont.loadFromFile(cinfo.mainFontInfo.file))
    {
        ezLog::Error("Failed to load main font from file: %s", cinfo.mainFontInfo.file);
        return false;
    }

    setupLines();

    return true;
}

void lcpp::Console::shutdown()
{

}

void lcpp::Console::update( sf::Time elapsedTime )
{
    buildText();
}

void lcpp::Console::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{

}

void lcpp::Console::buildText()
{
    for(auto& text : m_lines)
    {
        ezStringBuilder tempBuilder;

        //TODO: implement!
        //tempBuilder.AppendFormat("%u: %s", text.getCharacterSize(), m_inputText.GetData());

        text.setString(tempBuilder.GetData());
    }
}


ezUInt32 lcpp::Console::calcNumLines()
{
    //TODO: Implement me.
    return 2;
}

void lcpp::Console::setupLines()
{
    const ezInt32 spacing = m_mainFont.getLineSpacing(m_mainFontInfo.size);
    const ezUInt32 numLines = calcNumLines();

    m_lines.Clear();
    m_lines.Reserve(numLines);
    m_lines.SetCount(numLines);

    for (ezUInt32 i = 0; i < m_lines.GetCount(); i++)
    {
        auto& text = m_lines[i];

        text.setFont(m_mainFont);
        text.setCharacterSize(m_mainFontInfo.size);
        text.setColor(m_mainFontInfo.color);
        text.setPosition(10.0f, 10.0f + (i * spacing) + (i * m_mainFontInfo.size));
    }
}
