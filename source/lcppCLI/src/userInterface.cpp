#include "stdafx.h"
#include "userInterface.h"
#include "exceptions.h"

void lcpp::UserInterface::initialize(const CInfo& cinfo)
{
    m_Window.create(cinfo.videoMode, cinfo.windowTitle);

    m_windowClearColor = cinfo.windowClearColor;

    if (!m_mainFont.loadFromFile(cinfo.fontInfo.file))
    {
        throw exceptions::InitializationFailed("Failed to load font from file!");
    }

    m_mainFontSize = cinfo.fontInfo.size;
    m_mainFontColor = cinfo.fontInfo.color;

    m_eventHandlers[sf::Event::Closed] = [&](const sf::Event& evt)
    {
        m_keepRunning = false;
    };

    m_eventHandlers[sf::Event::TextEntered] = [&](const sf::Event& evt)
    {
        ezLog::Info("Entered: %c", evt.text.unicode);
        if (evt.text.unicode == '\b')
        {
            m_inputText.Shrink(0, 1);
        }
        else if(evt.text.unicode > 20 && evt.text.unicode < 255) //TODO: only ASCII for now
        {
            m_inputText.Append(evt.text.unicode);
        }
    };

    m_eventHandlers[sf::Event::KeyPressed] = [&](const sf::Event& evt)
    {
        switch (evt.key.code)
        {
        case sf::Keyboard::Escape:
            stop();
            break;
        case sf::Keyboard::Return:
            m_info.setString("Return pressed!");
            break;
        default:
            break;
        }
    };

    setupLines();
}

void lcpp::UserInterface::shutdown()
{

}

void lcpp::UserInterface::run()
{
    sf::Clock clock;
    while (m_keepRunning)
    {
        update(clock.restart());
        draw();
    }

    m_Window.close();
}

void lcpp::UserInterface::update(sf::Time elapsedTime)
{
    sf::Event evt;
    while (m_Window.pollEvent(evt))
    {
        auto& eventHandler = m_eventHandlers[evt.type];
        if (eventHandler) eventHandler(evt);
    }

    buildText();
}

void lcpp::UserInterface::buildText()
{
    for(auto& text : m_lines)
    {
        ezStringBuilder tempBuilder;

        tempBuilder.AppendFormat("%u: %s", text.getCharacterSize(), m_inputText.GetData());

        text.setString(tempBuilder.GetData());
    }
}

void lcpp::UserInterface::draw()
{
    m_Window.clear(m_windowClearColor);
    //////////////////////////////////////////////////////////////////////////

    for (auto& line : m_lines)
    {
        m_Window.draw(line);
    }

    m_Window.draw(m_info);

    //////////////////////////////////////////////////////////////////////////
    m_Window.display();
}

size_t lcpp::UserInterface::calcNumLines()
{
    //TODO: Implement me.
    return 2;
}

void lcpp::UserInterface::setupLines()
{
    const size_t spacing = m_mainFont.getLineSpacing(m_mainFontSize);
    const size_t numLines = calcNumLines();

    m_lines.Clear();
    m_lines.Reserve(numLines);
    m_lines.SetCount(numLines);

    for (size_t i = 0; i < m_lines.GetCount(); i++)
    {
        auto& text = m_lines[i];

        text.setFont(m_mainFont);
        text.setCharacterSize(m_mainFontSize);
        text.setColor(m_mainFontColor);
        text.setPosition(10.0f, 10.0f + (i * spacing) + (i * m_mainFontSize));
    }
}
