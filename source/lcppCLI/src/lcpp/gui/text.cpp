#include "stdafx.h"
#include "lcpp/gui/text.h"

//////////////////////////////////////////////////////////////////////////

void lcpp::Text::append(ezUInt32 unicodeChar)
{
    m_builder.Append(unicodeChar);
    markModified();
}

void lcpp::Text::append(const char* cstring)
{
    m_builder.Append(cstring);
    markModified();
}

void lcpp::Text::append(const ezString& str)
{
    m_builder.Append(str.GetData());
    markModified();
}

void lcpp::Text::appendFormat(const char* format, ...)
{
    va_list args;
    va_start(args, &format);
    m_builder.AppendFormat(format, args);
    va_end(args);
    markModified();
}

void lcpp::Text::prepend(ezUInt32 unicodeChar)
{
    m_builder.Prepend(unicodeChar);
    markModified();
}

void lcpp::Text::prepend(const char* cstring)
{
    m_builder.Prepend(cstring);
    markModified();
}

void lcpp::Text::prepend(const ezString& str)
{
    m_builder.Prepend(str.GetData());
    markModified();
}

void lcpp::Text::prependFormat(const char* format, ...)
{
    va_list args;
    va_start(args, &format);
    m_builder.PrependFormat(format, args);
    va_end(args);
    markModified();
}

void lcpp::Text::shrink( ezUInt32 shrinkCharsFront, ezUInt32 shrinkCharsBack )
{
    m_builder.Shrink(shrinkCharsFront, shrinkCharsBack);
    markModified();
}

