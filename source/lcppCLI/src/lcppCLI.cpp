// lcppCLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <functional>

namespace lcpp
{
    class State;

    struct Id
    {
        size_t value;
        Id(size_t value) : value(value){}
    };

    class State
    {
        ezString m_name;
    public:
        typedef std::function<void(const State&, const State&)> TransitionCallback;

        State(const ezString& name) : m_name(name) {}

        Id registerCallback(TransitionCallback callback){ m_callbacks.PushBack(callback); return Id(m_callbacks.GetCount() - 1); }
        void unregisterCallback(Id id){ /* TODO: Implement me. */ }

        ezString name() const { return m_name; }
        void name( ezString value ) { m_name = value; }

    private:
        ezDynamicArray<TransitionCallback> m_callbacks;
    };

    struct StateMachineCInfo
    {
        ezMap<ezString, State*> m_states;
        ezResult addTransition(const ezString& from, const ezString& to) { return EZ_SUCCESS; }

        StateMachineCInfo()
        {
        }
    };

    class StateMachine
    {
        ezMap<ezString, State*> m_states;
    public:
        StateMachine(const StateMachineCInfo& cinfo) {}

        bool startTransition(const ezString& from, const ezString& to){ return true; }
    };

    void entryFunction()
    {
        //StateMachineCInfo info;
        //info.addTransition(new State("a"), new State("b"));
        //info.addTransition(new State("b"), new State("c"));
        //
        //StateMachine sm(info);
        //sm.startTransition("a", "b");
    } 
}

//int _tmain(int argc, _TCHAR* argv[])
//{
//    ezStartup::StartupCore();
//
//    lcpp::entryFunction();
//
//    ezStartup::ShutdownBase();
//
//    return 0;
//}

namespace
{
    template<typename T>
    class ScopeExit
    {
        T m_f;
    public:
        ScopeExit(const T& executableObject) : m_f(executableObject)
        {}

        ~ScopeExit() { m_f(); }
    };
}

int main(int argc, const char* argv[])
{
    ezStartup::StartupCore();
    ScopeExit<std::function<void()>> shutdown([](){ ezStartup::ShutdownBase(); });

    sf::RenderWindow window(sf::VideoMode(800, 600), "My First SFML Application");
    sf::Font font;

    static const size_t numLines = 10;
    ezHybridArray<sf::Text, numLines> lines;
    lines.SetCount(numLines);

    sf::Text info;
    info.setFont(font);
    info.setCharacterSize(20);
    info.setColor(sf::Color(200, 200, 200, 255));
    info.setPosition(10, window.getSize().y - info.getCharacterSize() - 5);

    const size_t startSize = 11;
    const size_t step = 2;
    const size_t spacing = 5;

    if (!font.loadFromFile("../../data/fonts/consola.ttf"))
    {
        return -1;
    }

    std::function<float(size_t)> calcPosY = [&](size_t index) -> float
    {
        if (index == 0) return spacing;
        const auto characterSize = startSize + index * step;
        return characterSize + spacing + calcPosY(index - 1);
    };

    for (size_t i = 0; i < lines.GetCount(); i++)
    {
        auto& text = lines[i];

        text.setFont(font);
        text.setCharacterSize(startSize + i * step);
        text.setColor(sf::Color::White);
        text.setPosition(10.0f, calcPosY(i));
    }

    sf::Event evt;

    ezStringBuilder builder;

    while (window.isOpen())
    {
        while (window.pollEvent(evt))
        {
            switch (evt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::TextEntered:
                if (evt.text.unicode == '\b')
                {
                    builder.Shrink(0, 1);
                }
                else
                {
                    builder.Append(evt.text.unicode);
                }
                break;
            case sf::Event::KeyPressed:
                switch (evt.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                case sf::Keyboard::Return:
                    info.setString("Return pressed!");
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        // clear, then render stuff
        window.clear();

        for (size_t i = 0; i < lines.GetCount(); i++)
        {
            auto& text = lines[i];

            ezStringBuilder tempBuilder;

            tempBuilder.AppendFormat("%u: %s", text.getCharacterSize(), builder.GetData());

            text.setString(tempBuilder.GetData());
            window.draw(text);
        }

        window.draw(info);

        // now display
        window.display();
    }
    return 0;
}
