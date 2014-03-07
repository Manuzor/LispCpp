#include "stdafx.h"

#include "lcpp/cli/exceptions.h"

#include "lcpp/gui/userInterface.h"
#include "lcpp/gui/console.h"


void lcpp::UserInterface::initialize(const CInfo& cinfo)
{
    m_Window.create(cinfo.videoMode, cinfo.windowTitle);

    m_windowClearColor = cinfo.windowClearColor;

    {
        Console::CInfo consoleCInfo;
        m_console = new Console();
        if (!m_console->initialize(consoleCInfo))
        {
            throw exceptions::InitializationFailed("Failed to initialize console!");
        }
    }

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
            //TODO: implement the following!
            //m_console->status().setString("Return pressed!");
            break;
        default:
            break;
        }
    };
}

void lcpp::UserInterface::shutdown()
{
    LCPP_DELETE(m_console);
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

    for (auto& callback : m_updateCallbacks)
    {
        if (callback)
            callback(elapsedTime);
    }
}

void lcpp::UserInterface::draw()
{
    m_Window.clear(m_windowClearColor);
    //////////////////////////////////////////////////////////////////////////

    m_Window.draw(*m_console);

    //////////////////////////////////////////////////////////////////////////
    m_Window.display();
}

lcpp::UserInterface::CallbackId lcpp::UserInterface::registerUpdateCallback( UpdateCallback callback )
{
    for (ezUInt32 i = 0; i < m_updateCallbacks.GetCount(); ++i)
    {
        if (!m_updateCallbacks[i])
        {
            m_updateCallbacks[i] = callback;
            return CallbackId(i);
        }
    }
    m_updateCallbacks.PushBack(callback);
    return CallbackId(m_updateCallbacks.GetCount() - 1);
}

void lcpp::UserInterface::unregisterUpdateCallback( CallbackId id )
{
    //TODO: check for valid id
    m_updateCallbacks[id.id] = nullptr;
}
