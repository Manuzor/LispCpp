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
    m_input.setColor(m_mainFontInfo.color);
    m_input.boundsMax(m_bounds);

    m_cursor.setSize(m_input.getCharacterSize());
    m_cursor.setFont(m_mainFont);

    return true;
}

void lcpp::Console::shutdown()
{
}

void lcpp::Console::update( sf::Time elapsedTime )
{
    //if (m_textData.isModified()) buildText();
    //if (m_input.isModified()) buildInputText();
    //if (m_infoData.isModified()) buildInfoText();
    m_input.update(elapsedTime);
}

void lcpp::Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_input);
    target.draw(m_cursor);
}

/*
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
    LCPP_SCOPE_EXIT{ m_input.unmarkModified(); };
    //TODO: calculate proper position here!
    m_input.setString(m_input.data());
}

void lcpp::Console::buildInfoText()
{
    LCPP_SCOPE_EXIT{ m_infoData.unmarkModified(); };
    //TODO: Implement me
}
*/
