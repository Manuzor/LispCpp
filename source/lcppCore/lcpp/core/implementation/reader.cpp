#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/foundation/conversion.h"
#include "lcpp/exceptions/exceptions.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/foundation/stringUtils.h"
#include "lcpp/core/builtIn/recursive_syntax.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/environment.h"

lcpp::Ptr<lcpp::Reader>
lcpp::Reader::create(const CInfo& cinfo)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), Reader)(cinfo);
}

lcpp::Reader::Reader(const CInfo& cinfo) :
    m_defaults(),
    m_separators(cinfo.separators),
    m_symbolDelimiters(cinfo.symbolDelimiters),
    m_pSyntaxCheckResult(&m_defaults.syntaxCheckResult)
{
}

lcpp::Reader::~Reader()
{
}

void
lcpp::Reader::initialize()
{
    m_pSyntaxCheckResult->reset();

    auto pEnv = LispRuntime::instance()->syntaxEnvironment();
    Ptr<LispSymbol> pSymbol;

#define LCPP_ADD_SYNTAX_TO_ENVIRONMENT(name, funcPtr) pSymbol = LispSymbol::create(name); \
    pEnv->add(pSymbol, LispSyntax_BuiltIn::create(pSymbol, funcPtr));

    LCPP_ADD_SYNTAX_TO_ENVIRONMENT("define", &syntax::define);
    LCPP_ADD_SYNTAX_TO_ENVIRONMENT("set!", &syntax::set);
    LCPP_ADD_SYNTAX_TO_ENVIRONMENT("lambda", &syntax::lambda);
    LCPP_ADD_SYNTAX_TO_ENVIRONMENT("if", &syntax::if_);
    LCPP_ADD_SYNTAX_TO_ENVIRONMENT("and", &syntax::and);
    LCPP_ADD_SYNTAX_TO_ENVIRONMENT("or", &syntax::or);
    LCPP_ADD_SYNTAX_TO_ENVIRONMENT("assert", &syntax::assertion);

#undef LCPP_ADD_SYNTAX_TO_ENVIRONMENT
}

lcpp::Ptr<lcpp::LispObject>
lcpp::Reader::read(const ezString& inputString, bool resetSyntaxChecker)
{
    auto input = inputString.GetIteratorFront();
    return read(input, resetSyntaxChecker);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::Reader::read(ezStringIterator& input, bool resetSyntaxChecker)
{
    if(resetSyntaxChecker)
    {
        m_pSyntaxCheckResult->reset();
    }

    skipSeparators(input);

    if(!input.IsValid())
    {
        return LCPP_VOID;
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
    }

    return parseAtom(input);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::Reader::parseAtom(ezStringIterator& input)
{
    // Special case for + and - characters, since the ezEngine parses (+ 1) as +1...
    auto ch = input.GetCharacter();

    if (ch == '+' || ch == '-')
    {
        auto copy = input;

        ezStringBuilder symbol;
        symbol.Append(ch);

        ++copy;
        ch = copy.GetCharacter();
        while(true)
        {
            if(isSeparator(ch) || contains("()", char(ch)) || !copy.IsValid())
            {
                // The + or - characters stand alone, which means they're meant to be a symbol.
                while(input.GetData() != copy.GetData())
                {
                    advance(input);
                }
                return LispSymbol::create(symbol);
            }
            if (isDigit(ch))
            {
                // The +'s or -'s are sign changers of the digit we just encountered.
                // Abort reading as symbol.
                break;
            }
            symbol.Append(ch);
            ++copy;
            if(copy.IsValid())
            {
                ch = copy.GetCharacter();
            }
        }
    }

    // Try parsing for an integer first, then a number, then a symbol
    const char* lastPos = nullptr;
    LispInteger::Number_t integer;
    auto result = to(input, integer, &lastPos);

    // The string contains a number, but it is a floating point number; reparse.
    if (result.IsSuccess())
    {
        LCPP_SCOPE_EXIT{
            while(input.GetData() != lastPos)
            {
                advance(input);
            }
        };

        if(lastPos[0] == '.')
        {
            LispNumber::Number_t number;
            auto result = to(input, number, &lastPos);
            EZ_ASSERT(result.IsSuccess(), "An integer of the form '123.' should be parsed as float!");
            return LispNumber::create(number);
        }

        return LispInteger::create(integer);
    }

    return parseSymbol(input);
}

lcpp::Ptr<lcpp::LispInteger>
lcpp::Reader::parseInteger(ezStringIterator& input)
{
    skipSeparators(input);
    LispInteger::Number_t integer;
    auto result = to(input, integer);
    if (!result.IsSuccess())
    {
        throw exceptions::InvalidInput("Unable to parse an integer from the input.");
    }
    
    return LispInteger::create(integer);
}

lcpp::Ptr<lcpp::LispNumber>
lcpp::Reader::parseNumber(ezStringIterator& input)
{
    skipSeparators(input);
    LispNumber::Number_t number;
    auto result = to(input, number);
    if(!result.IsSuccess())
    {
        throw exceptions::InvalidInput("Unable to parse a number from the input.");
    }
    return LispNumber::create(number);
}

lcpp::Ptr<lcpp::LispSymbol>
lcpp::Reader::parseSymbol(ezStringIterator& input)
{
    skipSeparators(input);
    {
        // Test if the input string can be parsed as int, which should not be possible
        LispInteger::Number_t integer;
        if(to(input, integer).IsSuccess())
        {
            throw exceptions::InvalidInput("Invalid input: A number is not a symbol!");
        }
    }

    // Parse for a scheme symbol
    ezStringBuilder symbol;

    auto ch = input.GetCharacter();
    while(input.IsValid() && !isSymbolDelimiter(ch))
    {
        symbol.Append(ch);
        advance(input);
        ch = input.GetCharacter();
    }

    EZ_ASSERT(!symbol.IsEmpty(), "parsed symbol is not supposed to be empty!");

    return LispSymbol::create(symbol);
}


lcpp::Ptr<lcpp::LispString>
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
        return LispString::create("");
    }

    ezStringBuilder str;
    do
    {
        str.Append(ch);
        advance(input);
        if (!input.IsValid())
        {
            throw exceptions::InvalidInput("No closing \" found for string!");
        }
        
        ch = input.GetCharacter();
    } while(ch != '"');

    // consume trailing "
    advance(input);
    return LispString::create(str);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::Reader::parseList(ezStringIterator& input)
{
    skipSeparators(input);
    if(!input.StartsWith("("))
    {
        throw exceptions::InvalidInput("Input is not a valid cons/list!");
    }

    // skip first ( character
    advance(input);

    skipSeparators(input);
    auto ch = input.GetCharacter();

    if(ch == ')')
    {
        advance(input);
        return LCPP_NIL;
    }

    auto car = read(input, false);
    auto cdr = parseListHelper(input);

    if(car->is<LispSymbol>())
    {
        auto pSymbol = car.cast<LispSymbol>();
        Ptr<LispObject> pSyntax;
        auto result = LispRuntime::instance()->syntaxEnvironment()->get(pSymbol, pSyntax);
        if(result.IsSuccess() && pSyntax->is<LispSyntax>())
        {
            car = pSyntax;
        }
    }

    return LispCons::create(car, cdr);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::Reader::parseListHelper(ezStringIterator& input)
{
    skipSeparators(input);
    auto ch = input.GetCharacter();

    if(ch == ')')
    {
        advance(input);
        return LCPP_NIL;
    }

    auto car = read(input, false);
    auto cdr = parseListHelper(input);

    return LispCons::create(car, cdr);
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
lcpp::Reader::isSymbolDelimiter(ezUInt32 character)
{
    return contains(m_symbolDelimiters, character) || isSeparator(character);
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
    // Make a copy of the input iterator.
    ezStringIterator iter = input;

    // Save the current pointer to the actual syntax checker...
    auto temp = m_pSyntaxCheckResult;
    // ... and restore it before we return.
    LCPP_SCOPE_EXIT{ m_pSyntaxCheckResult = temp; };

    SyntaxCheckResult result;
    // Set the internal syntax checker to our result so it uses the correct cursor.
    m_pSyntaxCheckResult = &result;

    for(; iter.IsValid(); advance(iter))
    {
        auto ch = iter.GetCharacter();

        if(!isSeparator(ch))
        {
            result.isPureWhitespace = false;
        }

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
        m_pSyntaxCheckResult->cursor.advance();
        ++count;
    }
    ++iter;
    ++m_pSyntaxCheckResult->cursor;

    return count;
}
