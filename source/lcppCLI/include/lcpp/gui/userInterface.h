#pragma once

#include "lcpp/gui/fontInfo.h"

namespace lcpp
{
    class Console;

    class UserInterface
    {
    public:
        typedef std::function<void(sf::Time)> UpdateCallback;

        struct CInfo
        {
            sf::VideoMode videoMode;
            const char* windowTitle;
            FontInfo fontInfo;
            sf::Color windowClearColor;

            CInfo() :
                videoMode(800, 600, 32),
                windowTitle(g_ApplicationTitle),
                fontInfo(),
                windowClearColor(0, 0, 0)
            {
            }
        };

        struct CallbackId
        {
            ezUInt32 id;
            CallbackId(ezUInt32 id) : id(id) {}
        };
    private:
        sf::RenderWindow m_Window;
        sf::Color m_windowClearColor;

        Console* m_console;

        ezStringBuilder m_inputText;

        ezDynamicArray<UpdateCallback> m_updateCallbacks;
    public:
        UserInterface() : m_console(nullptr) {}
        ~UserInterface(){}

        void initialize(const CInfo& cinfo);
        void shutdown();

        void run();

        void stop() { m_keepRunning = false; };

        CallbackId registerUpdateCallback(UpdateCallback callback);
        void unregisterUpdateCallback(CallbackId id);

    protected:

        void update(sf::Time elapsedTime);
        void draw();

    private:
        typedef std::function<void(const sf::Event& event)> EventHandler;

        EventHandler m_eventHandlers[sf::Event::Count];
        bool m_keepRunning;

    };
}
