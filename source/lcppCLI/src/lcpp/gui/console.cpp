#include "stdafx.h"
#include "lcpp/gui/console.h"


bool lcpp::Console::initialize()
{
    if (!m_mainFont.loadFromFile(m_mainFontInfo.file))
    {
        ezLog::Error("Failed to load main font from file: %s", m_mainFontInfo.file);
        return false;
    }

    m_input.setFont(m_mainFont);

    setupLines();

    return true;
}

void lcpp::Console::shutdown()
{

}

void lcpp::Console::update( sf::Time elapsedTime )
{
    if (m_textData.isModified()) buildText();
    if (m_inputData.isModified()) buildInputText();
    if (m_infoData.isModified()) buildInfoText();
}

void lcpp::Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_input);
}

void lcpp::Console::buildText()
{
    LCPP_SCOPE_EXIT{ m_textData.unmarkModified(); };
    for(auto& text : m_textLines)
    {
        ezStringBuilder tempBuilder;

        //TODO: implement!
        //tempBuilder.AppendFormat("%u: %s", text.getCharacterSize(), m_inputText.GetData());

        text.setString(tempBuilder.GetData());
    }
}

void lcpp::Console::buildInputText()
{
    LCPP_SCOPE_EXIT{ m_inputData.unmarkModified(); };
    //TODO: calculate proper position here!
    m_input.setString(m_inputData.data());
}

void lcpp::Console::buildInfoText()
{
    LCPP_SCOPE_EXIT{ m_infoData.unmarkModified(); };
    //TODO: Implement me
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

    m_textLines.Clear();
    m_textLines.Reserve(numLines);
    m_textLines.SetCount(numLines);

    for (ezUInt32 i = 0; i < m_textLines.GetCount(); i++)
    {
        auto& text = m_textLines[i];

        text.setFont(m_mainFont);
        text.setCharacterSize(m_mainFontInfo.size);
        text.setColor(m_mainFontInfo.color);
        text.setPosition(10.0f, 10.0f + (i * spacing) + (i * m_mainFontInfo.size));
    }
}
