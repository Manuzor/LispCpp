#pragma once

namespace lcpp
{
    class Cursor : public sf::Drawable, public sf::Transformable
    {
    public:
        Cursor() :
            m_actualCursor()
        {
        }

        void setSize(ezUInt32 size) { m_actualCursor.setCharacterSize(size); }

        void setFont(const sf::Font& font) { m_actualCursor.setFont(font); m_actualCursor.setString("|"); }

        virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const
        {
            target.draw(m_actualCursor);
        }

    private:

        sf::Text m_actualCursor;
    };
}