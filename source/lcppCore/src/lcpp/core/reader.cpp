#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/foundation/conversion.h"
#include "lcpp/exceptions/exceptions.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/foundation/stringUtils.h"


lcpp::Reader::Reader() :
    m_separators(CInfo().separators),
    m_defaultFactory(),
    m_pFactory(&m_defaultFactory)
{
}

lcpp::Reader::Reader(const CInfo& cinfo) :
    m_separators(cinfo.separators),
    m_defaultFactory(),
    m_pFactory(cinfo.pFactory ? cinfo.pFactory : &m_defaultFactory)
{
}

lcpp::Reader::~Reader()
{
}

lcpp::SchemeObject* lcpp::Reader::read(const ezString& inputString)
{
    SchemeObject* pResultObject = nullptr;
    auto input = inputString.GetIteratorFront();

    skipSeparators(input);

    if(!input.IsValid())
    {
        throw exceptions::InvalidInput("Input string only contained whitespace!");
    }

    switch(input.GetCharacter())
    {
    case ')':
        throw exceptions::InvalidInput("Unexpected character ')'.");
    case '\'':
        // TODO read quote
        break;
    case '"':
        pResultObject = parseString(input);
        break;
    case '(':
        // TODO read list
        pResultObject = parseList(input);
        break;
    default:
        // Try parsing for an integer first, then a number, then a symbol
        {
            const char* lastPos = nullptr;
            SchemeInteger::Number_t integer;
            auto result = to(input, integer, &lastPos);

            // The string contains a number, but it is a floating point number; reparse.
            if (result.IsSuccess())
            {
                LCPP_SCOPE_EXIT{ input.SetCurrentPosition(lastPos); };

                if(lastPos[0] == '.')
                {
                    SchemeNumber::Number_t number;
                    auto result = to(input, number, &lastPos);
                    EZ_ASSERT(result.IsSuccess(), "An integer of the form '123.' should be parsed as float!");
                    pResultObject = m_pFactory->createNumber(number);
                    break;
                }

                pResultObject = m_pFactory->createInteger(integer);
                break;
            }
            pResultObject = parseSymbol(input);
        }
        break;
    }

    return pResultObject;
}

void lcpp::Reader::skipSeparators(ezStringIterator& iter)
{
    while(iter.IsValid() && isSeparator(iter.GetCharacter()))
    {
        ++iter;
    }
}

bool lcpp::Reader::isSeparator(ezUInt32 character)
{
    return contains(m_separators, character);
}

lcpp::SchemeInteger* lcpp::Reader::parseInteger(const ezString& inputString)
{
    SchemeInteger::Number_t integer;
    auto result = to(inputString, integer);
    if (!result.IsSuccess())
    {
        throw exceptions::InvalidInput("Unable to parse an integer from the input.");
    }
    
    return m_pFactory->createInteger(integer);
}

lcpp::SchemeNumber* lcpp::Reader::parseNumber(const ezString& inputString)
{
    SchemeNumber::Number_t number;
    auto result = to(inputString, number);
    if(!result.IsSuccess())
    {
        throw exceptions::InvalidInput("Unable to parse a number from the input.");
    }
    return m_pFactory->createNumber(number);
}

lcpp::SchemeSymbol* lcpp::Reader::parseSymbol(const ezString& inputString)
{
    auto input = inputString.GetIteratorFront();

    // Parse for a scheme symbol
    ezStringBuilder symbol;

    while(input.IsValid() && !isSeparator(input.GetCharacter()))
    {
        symbol.Append(input.GetCharacter());
        ++input;
    }

    EZ_ASSERT(!symbol.IsEmpty(), "parsed symbol is not supposed to be empty!");

    return m_pFactory->createSymbol(symbol);
}

lcpp::SchemeString* lcpp::Reader::parseString(const ezString& inputString)
{
    if (!inputString.StartsWith("\""))
    {
        return nullptr;
    }

    auto input = inputString.GetIteratorFront();
    // skip the " character
    ++input;

    auto ch = input.GetCharacter();

    if(ch == '"')
    {
        return m_pFactory->createString("");
    }

    ezStringBuilder str;
    do
    {
        str.Append(ch);
        ++input;
        ch = input.GetCharacter();
    } while(input.IsValid() && ch != '"');

    return m_pFactory->createString(str);
}

lcpp::SchemeCons* lcpp::Reader::parseList(const ezString& inputString)
{
    return nullptr;
}
