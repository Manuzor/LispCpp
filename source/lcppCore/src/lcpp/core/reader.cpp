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
    m_pFactory = nullptr;
}

lcpp::SchemeObject& lcpp::Reader::read(const ezString& inputString)
{
    auto input = inputString.GetIteratorFront();
    return read(input);
}

lcpp::SchemeObject& lcpp::Reader::read(ezStringIterator& input)
{
    skipSeparators(input);

    if(!input.IsValid())
    {
        throw exceptions::InvalidInput("Input string only contained separators!");
    }

    switch(input.GetCharacter())
    {
    case ')':
        throw exceptions::InvalidInput("Unexpected character ')'.");
    case '\'':
        // TODO read quote
        break;
    case '"':
        return parseString(input);
        break;
    case '(':
        return parseList(input);
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
                    return m_pFactory->createNumber(number);
                }

                return m_pFactory->createInteger(integer);
            }
            return parseSymbol(input);
        }
        break;
    }

    return SCHEME_NIL;
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

lcpp::SchemeInteger& lcpp::Reader::parseInteger(ezStringIterator& input)
{
    skipSeparators(input);
    SchemeInteger::Number_t integer;
    auto result = to(input, integer);
    if (!result.IsSuccess())
    {
        throw exceptions::InvalidInput("Unable to parse an integer from the input.");
    }
    
    return m_pFactory->createInteger(integer);
}

lcpp::SchemeNumber& lcpp::Reader::parseNumber(ezStringIterator& input)
{
    skipSeparators(input);
    SchemeNumber::Number_t number;
    auto result = to(input, number);
    if(!result.IsSuccess())
    {
        throw exceptions::InvalidInput("Unable to parse a number from the input.");
    }
    return m_pFactory->createNumber(number);
}

lcpp::SchemeSymbol& lcpp::Reader::parseSymbol(ezStringIterator& input)
{
    skipSeparators(input);
    {
        // Test if the input string can be parsed as int, which should not be possible
        SchemeInteger::Number_t integer;
        if(to(input, integer).IsSuccess())
        {
            throw exceptions::InvalidInput("Invalid input: A number is not a symbol!");
        }
    }

    // Parse for a scheme symbol
    ezStringBuilder symbol;

    auto ch = input.GetCharacter();
    while(input.IsValid() && ch != ')' && !isSeparator(input.GetCharacter()))
    {
        symbol.Append(input.GetCharacter());
        ++input;
        ch = input.GetCharacter();
    }

    EZ_ASSERT(!symbol.IsEmpty(), "parsed symbol is not supposed to be empty!");

    return m_pFactory->createSymbol(symbol);
}


lcpp::SchemeString& lcpp::Reader::parseString(ezStringIterator& input)
{
    skipSeparators(input);
    if(!input.StartsWith("\""))
    {
        throw exceptions::InvalidInput("Input is not a valid string!");
    }

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

lcpp::SchemeObject& lcpp::Reader::parseList(ezStringIterator& input)
{
    skipSeparators(input);
    if(!input.StartsWith("("))
    {
        throw exceptions::InvalidInput("Input is not a valid cons/list!");
    }

    // skip first ( character
    ++input;

    return parseListHelper(input);
}

lcpp::SchemeObject& lcpp::Reader::parseListHelper(ezStringIterator& input)
{
    skipSeparators(input);
    auto ch = input.GetCharacter();

    if(ch == ')')
    {
        ++input;
        return SCHEME_NIL;
    }

    auto& car = read(input);
    auto& cdr = parseListHelper(input);

    return m_pFactory->createCons(car, cdr);
}
