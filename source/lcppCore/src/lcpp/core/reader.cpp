#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/foundation/conversion.h"
#include "lcpp/exceptions/exceptions.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/foundation/stringUtils.h"


lcpp::Reader::Reader() :
    defaults(),
    m_separators(CInfo().separators),
    m_factory(defaults.factory),
    m_cursor(defaults.cursor)
{
}

lcpp::Reader::Reader(const CInfo& cinfo) :
    defaults(),
    m_separators(cinfo.separators),
    m_factory(cinfo.pFactory ? *cinfo.pFactory : defaults.factory),
    m_cursor(cinfo.pCursor ? *cinfo.pCursor : defaults.cursor)
{
}

lcpp::Reader::~Reader()
{
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::Reader::read(const ezString& inputString, bool resetCursor)
{
    auto input = inputString.GetIteratorFront();
    return read(input, resetCursor);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::Reader::read(ezStringIterator& input, bool resetCursor)
{
    if (resetCursor)
    {
        m_cursor.reset();
    }

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
        throw exceptions::NotImplemented("Not supporting quote yet.");
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
                    return m_factory.createNumber(number);
                }

                return m_factory.createInteger(integer);
            }
            return parseSymbol(input);
        }
        break;
    }

    return SCHEME_NIL_PTR;
}

lcpp::Ptr<lcpp::SchemeInteger>
lcpp::Reader::parseInteger(ezStringIterator& input)
{
    skipSeparators(input);
    SchemeInteger::Number_t integer;
    auto result = to(input, integer);
    if (!result.IsSuccess())
    {
        throw exceptions::InvalidInput("Unable to parse an integer from the input.");
    }
    
    return m_factory.createInteger(integer);
}

lcpp::Ptr<lcpp::SchemeNumber>
lcpp::Reader::parseNumber(ezStringIterator& input)
{
    skipSeparators(input);
    SchemeNumber::Number_t number;
    auto result = to(input, number);
    if(!result.IsSuccess())
    {
        throw exceptions::InvalidInput("Unable to parse a number from the input.");
    }
    return m_factory.createNumber(number);
}

lcpp::Ptr<lcpp::SchemeSymbol>
lcpp::Reader::parseSymbol(ezStringIterator& input)
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
    while(input.IsValid() && ch != ')' && !isSeparator(ch))
    {
        symbol.Append(ch);
        advance(input);
        ch = input.GetCharacter();
    }

    EZ_ASSERT(!symbol.IsEmpty(), "parsed symbol is not supposed to be empty!");

    return m_factory.createSymbol(symbol);
}


lcpp::Ptr<lcpp::SchemeString>
lcpp::Reader::parseString(ezStringIterator& input)
{
    skipSeparators(input);
    if(!input.StartsWith("\""))
    {
        throw exceptions::InvalidInput("Input is not a valid string!");
    }

    // skip the " character
    advance(input);

    auto ch = input.GetCharacter();

    if(ch == '"')
    {
        advance(input);
        return m_factory.createString("");
    }

    ezStringBuilder str;
    do
    {
        str.Append(ch);
        advance(input);
        ch = input.GetCharacter();
    } while(input.IsValid() && ch != '"');

    // consume trailing "
    advance(input);
    return m_factory.createString(str);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::Reader::parseList(ezStringIterator& input)
{
    skipSeparators(input);
    if(!input.StartsWith("("))
    {
        throw exceptions::InvalidInput("Input is not a valid cons/list!");
    }

    // skip first ( character
    advance(input);

    return parseListHelper(input);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::Reader::parseListHelper(ezStringIterator& input)
{
    skipSeparators(input);
    auto ch = input.GetCharacter();

    if(ch == ')')
    {
        advance(input);
        return SCHEME_NIL_PTR;
    }

    auto& car = read(input, false);
    auto& cdr = parseListHelper(input);

    return m_factory.createCons(car, cdr);
}

ezUInt32
lcpp::Reader::skipSeparators(ezStringIterator& iter)
{
    ezUInt32 count = 0;
    auto ch = iter.GetCharacter();
    while(iter.IsValid() && isSeparator(ch) || isComment(ch))
    {
        LCPP_SCOPE_EXIT{ ch = iter.GetCharacter(); };

        if (isComment(ch))
        {
            skipToFirstNewLine(iter);
        }
        
        ++count;
        advance(iter);
    }
    return count;
}

void
lcpp::Reader::skipToFirstNewLine(ezStringIterator& iter)
{
    while(!isNewLine(iter.GetCharacter()))
    {
        advance(iter);
    }
}

bool
lcpp::Reader::isSeparator(ezUInt32 character)
{
    return contains(m_separators, character);
}

bool
lcpp::Reader::isNewLine(ezUInt32 character)
{
    return character == '\n';
}

bool lcpp::Reader::isComment(ezUInt32 character)
{
    return character == ';';
}

lcpp::Reader::SyntaxCheckResult
lcpp::Reader::checkBasicSyntax(const ezStringIterator& input)
{
    m_cursor.reset();

    ezStringIterator iter = input;
    SyntaxCheckResult result(m_cursor);

    for(; iter.IsValid(); advance(iter))
    {
        auto ch = iter.GetCharacter();

        switch(ch)
        {
        case '(':
            result.hasParenthesis = true;
            result.parenthesisBalance++;
            break;
        case ')':
            result.parenthesisBalance--;

            if(!result.hasParenthesis || result.parenthesisBalance < 0)
            {
                result.info = "Invalid closing parenthesis!";
                return result;
            }
            break;
        default:
            break;
        }
    }

    result.valid = true;
    return result;
}

ezUInt8
lcpp::Reader::advance(ezStringIterator& iter)
{
    ezUInt8 count = 1;
    if(isNewLine(iter.GetCharacter()))
    {
        m_cursor.advance();
        ++count;
    }
    ++iter;
    ++m_cursor;

    return count;
}
