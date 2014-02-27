#pragma once

namespace lcpp
{
    struct FontInfo
    {
        const char* file;
        sf::Uint32 size;
        sf::Color color;

        FontInfo() :
            file("../../data/fonts/consola.ttf"),
            size(17),
            color(sf::Color::White)
        {

        }
    };

    class UserInterface
    {
    public:
        struct CInfo
        {
            sf::VideoMode videoMode;
            const char* windowTitle;
            FontInfo fontInfo;
            sf::Color windowClearColor;

            CInfo() :
                videoMode(800, 600),
                windowTitle(g_ApplicationTitle),
                fontInfo(),
                windowClearColor(0, 0, 0)
            {
            }
        };
    private:
        sf::RenderWindow m_Window;
        sf::Font m_mainFont;
        sf::Uint32 m_mainFontSize;
        sf::Color m_mainFontColor;
        sf::Color m_windowClearColor;

        ezDynamicArray<sf::Text> m_lines;
        sf::Text m_info;

        ezStringBuilder m_text;
        ezStringBuilder m_inputText;
    public:
        UserInterface(){}
        ~UserInterface(){}

        void initialize(const CInfo& cinfo);
        void shutdown();

        void run();

        void stop() { m_keepRunning = false; };

    protected:

        void update(sf::Time elapsedTime);
        void buildText();
        void draw();

    private:
        typedef std::function<void(const sf::Event& event)> EventHandler;
        EventHandler m_eventHandlers[sf::Event::Count];
        bool m_keepRunning;

        void handleEvents();

        size_t calcNumLines();
        void setupLines();

    };
}
