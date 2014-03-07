#pragma once

namespace lcpp
{
    struct FontInfo
    {
        const char* file;
        sf::Uint32 size;
        sf::Color color;

        FontInfo() :
            file("../../data/fonts/consola.ttf"), // Default font
            size(17),
            color(sf::Color::White)
        {
        }
    };
}