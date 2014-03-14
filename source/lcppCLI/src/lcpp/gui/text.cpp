#include "stdafx.h"
#include "lcpp/gui/text.h"

//////////////////////////////////////////////////////////////////////////

void
lcpp::Text::append(ezUInt32 unicodeChar)
{
    if (willOverflow(unicodeChar))
    {
        m_builder.Append('\n');
    }
    m_builder.Append(unicodeChar);
    markModified();
}

void
lcpp::Text::append(const char* cstring)
{
    if (willOverflow(cstring))
    {
        m_builder.Append('\n');
    }
    m_builder.Append(cstring);
    markModified();
}

void
lcpp::Text::append(const ezString& str)
{
    append(str.GetData());
}

void
lcpp::Text::appendFormat(const char* format, ...)
{
    ezStringBuilder tempBuilder;
    va_list args;
    va_start(args, &format);
    tempBuilder.AppendFormat(format, args);
    va_end(args);
    if (willOverflow(tempBuilder.GetData()))
    {
        m_builder.Append('\n');
    }
    m_builder.Append(tempBuilder.GetData());
    markModified();
}

void
lcpp::Text::prepend(ezUInt32 unicodeChar)
{
    m_builder.Prepend(unicodeChar);
    markModified();
}

void
lcpp::Text::prepend(const char* cstring)
{
    m_builder.Prepend(cstring);
    markModified();
}

void
lcpp::Text::prepend(const ezString& str)
{
    prepend(str.GetData());
}

void
lcpp::Text::prependFormat(const char* format, ...)
{
    va_list args;
    va_start(args, &format);
    m_builder.PrependFormat(format, args);
    va_end(args);
    markModified();
}

void
lcpp::Text::shrink( ezUInt32 shrinkCharsFront, ezUInt32 shrinkCharsBack )
{
    m_builder.Shrink(shrinkCharsFront, shrinkCharsBack);
    markModified();
}

bool
lcpp::Text::willOverflow(ezUInt32 unicodeChar) const
{
    EZ_ASSERT(m_text.getFont() != nullptr, "Text has no font attached!");

    const auto& glyph = m_text.getFont()->getGlyph(unicodeChar, m_text.getCharacterSize(), false);

    return m_text.getGlobalBounds().width + glyph.bounds.width >= m_boundsMax.width;
}

bool
lcpp::Text::willOverflow(const char* text) const
{
    return true;
}

void lcpp::Text::update(sf::Time elapsedTime)
{
    if (isModified()) m_text.setString(m_builder.GetData());
}

void lcpp::Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_text);
}
