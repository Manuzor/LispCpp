#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/foundation/stringUtils.h"

#include "lcpp/core/exceptions/invalidInputException.h"

namespace lcpp
{
    namespace reader
    {

        Ptr<LispObject> read(Ptr<LispObject> pContinuation)
        {
            typeCheck(pContinuation, Type::Continuation);

            auto& stack = cont::getStack(pContinuation);
            auto pStream = stack.get(0);
            typeCheck(pStream, Type::Stream);

            detail::skipSeparators(pStream);

            if (!stream::isValid(pStream))
            {
                stack.push(LCPP_pVoid);
                return LCPP_pNil;
            }

            switch(stream::getCharacter(pStream))
            {
            case ')':
                throw exceptions::InvalidInput("Unexpected character ')'.");
            case '"':
                cont::setFunction(pContinuation, &detail::readString);
                return pContinuation;
            case '(':
                cont::setFunction(pContinuation, &detail::readList);
                return pContinuation;
            }

            cont::setFunction(pContinuation, &detail::readAtom);
            return pContinuation;
        }

        namespace detail
        {
            Ptr<LispObject> readAtom(Ptr<LispObject> pContinuation)
            {
                typeCheck(pContinuation, Type::Continuation);
                auto& stack = cont::getStack(pContinuation);

                auto pStream = stack.get(0);
                typeCheck(pStream, Type::Stream);

                auto& iter = stream::getIterator(pStream);

                // Special case for + and - characters, since the ezEngine parses (+ 1) as +1...
                auto ch = stream::getCharacter(pStream);

                if(ch == '+' || ch == '-')
                {
                    auto copy = iter;

                    ezStringBuilder symbolValue;
                    symbolValue.Append(ch);

                    ++copy;
                    ch = copy.GetCharacter();
                    while(true)
                    {
                        if(isSeparator(ch) || isSymbolDelimiter(ch) || !copy.IsValid())
                        {
                            // The + or - characters stand alone, which means they're meant to be a symbol.
                            while(iter.GetData() != copy.GetData())
                            {
                                advance(pStream);
                            }
                            stack.push(symbol::create(symbolValue));
                            return LCPP_pNil;
                        }
                        if(isDigit(ch))
                        {
                            // The +'s or -'s are sign changers of the digit we just encountered.
                            // Abort reading as symbol.
                            break;
                        }
                        symbolValue.Append(ch);
                        ++copy;
                        if(copy.IsValid())
                        {
                            ch = copy.GetCharacter();
                        }
                    }
                }

                // Try parsing for an integer first, then a number, then a symbol
                const char* lastPos = nullptr;
                number::Integer_t integer;
                auto result = to(iter, integer, &lastPos);

                // The string contains a number, but it is a floating point number; reparse.
                if(result.Succeeded())
                {
                    LCPP_SCOPE_EXIT
                    {
                        while(iter.GetData() != lastPos)
                        {
                            advance(pStream);
                        }
                    };

                    if(lastPos[0] == '.')
                    {
                        number::Float_t theFloat;
                        auto result = to(iter, theFloat, &lastPos);
                        EZ_ASSERT(result.Succeeded(), "An integer of the form '123.' should be parsed as float!");
                        stack.push(number::create(theFloat));
                        return LCPP_pNil;
                    }

                    stack.push(number::create(integer));
                    return LCPP_pNil;
                }

                cont::setFunction(pContinuation, &readSymbol);
                return pContinuation;
            }

            Ptr<LispObject> readSymbol(Ptr<LispObject> pContinuation)
            {
                typeCheck(pContinuation, Type::Continuation);

                auto pStream = cont::getStack(pContinuation).get(0);
                typeCheck(pStream, Type::Stream);

                skipSeparators(pStream);

                // Parse for a scheme symbol
                ezStringBuilder theSymbol;

                auto ch = stream::getCharacter(pStream);
                while(stream::isValid(pStream) && !isSymbolDelimiter(ch))
                {
                    theSymbol.Append(ch);
                    advance(pStream);
                    ch = stream::getCharacter(pStream);
                }

                EZ_ASSERT(!theSymbol.IsEmpty(), "parsed symbol is not supposed to be empty!");

                cont::getStack(pContinuation).push(symbol::create(theSymbol));
                return LCPP_pNil;
            }

            Ptr<LispObject> readString(Ptr<LispObject> pContinuation)
            {
                typeCheck(pContinuation, Type::Continuation);

                auto pStream = cont::getStack(pContinuation).get(0);
                typeCheck(pStream, Type::Stream);

                LCPP_NOT_IMPLEMENTED;
            }

            Ptr<LispObject> readListHelper(Ptr<LispObject> pContinuation)
            {
                typeCheck(pContinuation, Type::Continuation);

                auto pStream = cont::getStack(pContinuation).get(0);
                typeCheck(pStream, Type::Stream);

                LCPP_NOT_IMPLEMENTED;
            }

            Ptr<LispObject> readList(Ptr<LispObject> pContinuation)
            {
                typeCheck(pContinuation, Type::Continuation);

                auto pStream = cont::getStack(pContinuation).get(0);
                typeCheck(pStream, Type::Stream);

                LCPP_NOT_IMPLEMENTED;
            }

            //////////////////////////////////////////////////////////////////////////

            ezUInt32 skipSeparators(Ptr<LispObject> pStream)
            {
                typeCheck(pStream, Type::Stream);

                ezUInt32 count = 0;
                auto& iter = stream::getIterator(pStream);
                auto ch = stream::getCharacter(pStream);
                while(stream::isValid(pStream) && isSeparator(ch) || isCommentDelimiter(ch))
                {
                    LCPP_SCOPE_EXIT{ ch = iter.GetCharacter(); };

                    if(isCommentDelimiter(ch))
                    {
                        count += skipToFirstNewLine(pStream);
                    }

                    ++count;
                    advance(pStream);
                }
                return count;
            }

            LCPP_API_CORE_CONT ezUInt32 skipToFirstNewLine(Ptr<LispObject> pStream)
            {
                typeCheck(pStream, Type::Stream);

                auto count = ezUInt32(0);
                auto& iter = stream::getIterator(pStream);

                while(!isNewLine(iter.GetCharacter()))
                {
                    count += advance(pStream);
                }

                return count;
            }

            ezUInt32 advance(Ptr<LispObject> pStream)
            {
                typeCheck(pStream, Type::Stream);

                auto count = ezUInt32(1);
                auto character = stream::getCharacter(pStream);

                if(isNewLine(character))
                {
                    //m_pSyntaxCheckResult->cursor.lineBreak();
                }
                else
                {
                    //m_pSyntaxCheckResult->cursor.advance();
                }

                stream::next(pStream);

                return count;
            }

            bool isSeparator(ezUInt32 character)
            {
                static auto separators = ezString(" \t\r\n\v\f\a");

                return contains(separators, character);
            }

            bool isNewLine(ezUInt32 character)
            {
                return character == '\n';
            }

            bool isSymbolDelimiter(ezUInt32 character)
            {
                static auto symbolDelimiters = ezString("()");

                return contains(symbolDelimiters, character) || isSeparator(character);
            }

            bool isCommentDelimiter(ezUInt32 character)
            {
                return character == ';';
            }
        }
    }
}
