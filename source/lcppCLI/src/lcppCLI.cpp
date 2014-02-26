// lcppCLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <functional>

int main(int argc, const char* argv[])
{
    ezStartup::StartupCore();
    LCPP_SCOPE_EXIT { ezStartup::ShutdownBase(); };

    sf::RenderWindow window(sf::VideoMode(800, 600), "My First SFML Application");
    sf::Font font;

    static const size_t numLines = 16;
    ezDynamicArray<sf::Text> lines;
    lines.Reserve(numLines);
    lines.SetCount(numLines);

    sf::Text info;
    info.setFont(font);
    info.setCharacterSize(20);
    info.setColor(sf::Color(200, 200, 200, 255));
    info.setPosition(10, window.getSize().y - info.getCharacterSize() - 5.0f);

    const size_t startSize = 11;
    const size_t step = 2;
    const size_t spacing = 5;

    if (!font.loadFromFile("../../data/fonts/consola.ttf"))
    {
        return -1;
    }

    std::function<float(size_t)> calcPosY = [&](size_t index) -> float
    {
        if (index == 0) return float(spacing);
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

        for(auto& text : lines)
        {
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
