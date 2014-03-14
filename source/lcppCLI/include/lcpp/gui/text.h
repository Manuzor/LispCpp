#pragma once

namespace lcpp
{

    class Text : public sf::Drawable
    {
        bool m_isModified;
        ezStringBuilder m_builder;

        sf::FloatRect m_boundsMax;

        sf::Text m_text;
    public:
        inline Text() :
            m_isModified(false),
            m_boundsMax(),
            m_builder()
        {
        }
        inline ~Text() { m_builder.Clear(); }

        void append(ezUInt32 unicodeChar);
        void append(const char* cstring);
        void append(const ezString& str);
        void appendFormat(const char* format, ...);

        // Not supported right now!
    private:
        void prepend(ezUInt32 unicodeChar);
        void prepend(const char* cstring);
        void prepend(const ezString& str);
        void prependFormat(const char* format, ...);

    public:

        void shrink(ezUInt32 shrinkCharsFront, ezUInt32 shrinkCharsBack);

        inline void clear(){ m_builder.Clear(); }

        inline const char* data(){ return m_builder.GetData(); }

        inline void markModified(){ m_isModified = true; }
        inline void unmarkModified(){ m_isModified = false; }
        inline bool isModified() const { return m_isModified; }

        inline void boundsMax(sf::FloatRect width) { m_boundsMax = width; }
        inline sf::FloatRect boundsMax() const { return m_boundsMax; }

        inline void setFont(const sf::Font& font) { m_text.setFont(font); }
        inline const sf::Font* getFont() const { return m_text.getFont(); }

        inline void setCharacterSize(ezUInt32 size) { m_text.setCharacterSize(size); }
        inline ezUInt32 getCharacterSize() const { return m_text.getCharacterSize(); }

        inline void setColor(sf::Color color) { m_text.setColor(color); }
        inline sf::Color getColor() const { return m_text.getColor(); }

        void update(sf::Time elapsedTime);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:

        bool willOverflow(ezUInt32 unicodeChar) const;
        bool willOverflow(const char* text) const;


    };

}