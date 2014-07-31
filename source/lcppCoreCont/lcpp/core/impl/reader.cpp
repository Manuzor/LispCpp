#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/foundation/stringUtils.h"

#include "lcpp/core/exceptions/invalidInputException.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace reader
    {

        Ptr<LispObject> read(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);
            auto pState = cont::getRuntimeState(pCont)->getReaderState();

            auto pContParent = cont::getParent(pCont);
            auto pReturnStack = cont::getStack(pContParent);
            auto pStream = cont::getStack(pCont)->get(0);
            typeCheck(pStream, Type::Stream);

            detail::skipSeparators(pState, pStream);

            if (!stream::isValid(pStream))
            {
                pReturnStack->push(LCPP_pVoid);
                cont::getParent(pCont);
                LCPP_cont_return(pCont);
            }

            switch(stream::getCharacter(pStream))
            {
            case ')':
                throw exceptions::InvalidInput("Unexpected character ')'.");
            case '"':
                LCPP_cont_tailCall(pCont, &detail::readString);
            case '(':
                LCPP_cont_tailCall(pCont, &detail::readList);
            }

            LCPP_cont_tailCall(pCont, &detail::readAtom);
        }

        namespace detail
        {
            Ptr<LispObject> readAtom(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pState = cont::getRuntimeState(pCont)->getReaderState();

                auto pStream = cont::getStack(pCont)->get(0);
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
                        if(isSeparator(pState, ch) || isSymbolDelimiter(pState, ch) || !copy.IsValid())
                        {
                            // The + or - characters stand alone, which means they're meant to be a symbol.
                            while(iter.GetData() != copy.GetData())
                            {
                                advance(pState, pStream);
                            }

                            LCPP_cont_return(pCont, symbol::create(symbolValue));
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
                            advance(pState, pStream);
                        }
                    };

                    if(lastPos[0] == '.')
                    {
                        number::Float_t theFloat;
                        auto result = to(iter, theFloat, &lastPos);
                        EZ_ASSERT(result.Succeeded(), "An integer of the form '123.' should be parsed as float!");

                        LCPP_cont_return(pCont, number::create(theFloat));
                    }

                    LCPP_cont_return(pCont, number::create(integer));
                }


                LCPP_cont_tailCall(pCont, &readSymbol);
            }

            Ptr<LispObject> readSymbol(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pState = cont::getRuntimeState(pCont)->getReaderState();

                auto pStream = cont::getStack(pCont)->get(0);
                typeCheck(pStream, Type::Stream);

                skipSeparators(pState, pStream);

                // Parse for a scheme symbol
                ezStringBuilder theSymbol;

                auto ch = stream::getCharacter(pStream);
                while(stream::isValid(pStream) && !isSymbolDelimiter(pState, ch))
                {
                    theSymbol.Append(ch);
                    advance(pState, pStream);
                    ch = stream::getCharacter(pStream);
                }

                EZ_ASSERT(!theSymbol.IsEmpty(), "parsed symbol is not supposed to be empty!");

                LCPP_cont_return(pCont, symbol::create(theSymbol));
            }

            Ptr<LispObject> readString(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pStream = pStack->get(0);
                typeCheck(pStream, Type::Stream);

                LCPP_NOT_IMPLEMENTED;
            }

            Ptr<LispObject> readListHelper(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pStream = pStack->get(0);
                typeCheck(pStream, Type::Stream);

                LCPP_NOT_IMPLEMENTED;
            }

            Ptr<LispObject> readList(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pStream = pStack->get(0);
                typeCheck(pStream, Type::Stream);

                LCPP_NOT_IMPLEMENTED;
            }

            //////////////////////////////////////////////////////////////////////////

            ezUInt32 skipSeparators(Ptr<State> pState, Ptr<LispObject> pStream)
            {
                typeCheck(pStream, Type::Stream);

                ezUInt32 count = 0;
                auto& iter = stream::getIterator(pStream);
                auto ch = stream::getCharacter(pStream);
                while(stream::isValid(pStream) && isSeparator(pState, ch) || isCommentDelimiter(pState, ch))
                {
                    LCPP_SCOPE_EXIT{ ch = iter.GetCharacter(); };

                    if(isCommentDelimiter(pState, ch))
                    {
                        count += skipToFirstNewLine(pState, pStream);
                    }

                    ++count;
                    advance(pState, pStream);
                }
                return count;
            }

            ezUInt32 skipToFirstNewLine(Ptr<State> pState, Ptr<LispObject> pStream)
            {
                typeCheck(pStream, Type::Stream);

                auto count = ezUInt32(0);
                auto& iter = stream::getIterator(pStream);

                while(!isNewLine(pState, iter.GetCharacter()))
                {
                    count += advance(pState, pStream);
                }

                return count;
            }

            ezUInt32 advance(Ptr<State> pState, Ptr<LispObject> pStream)
            {
                typeCheck(pStream, Type::Stream);

                auto count = ezUInt32(1);
                auto character = stream::getCharacter(pStream);

                if(isNewLine(pState, character))
                {
                    pState->m_syntaxCheckResult.m_cursor.lineBreak();
                }
                else
                {
                    pState->m_syntaxCheckResult.m_cursor.advance();
                }

                stream::next(pStream);

                return count;
            }

            bool isSeparator(Ptr<State> pState, ezUInt32 character)
            {
                return contains(pState->m_separators, character);
            }

            bool isNewLine(Ptr<State> pState, ezUInt32 character)
            {
                return character == pState->m_newLineDelimiter;
            }

            bool isSymbolDelimiter(Ptr<State> pState, ezUInt32 character)
            {
                return contains(pState->m_symbolDelimiters, character) || isSeparator(pState, character);
            }

            bool isCommentDelimiter(Ptr<State> pState, ezUInt32 character)
            {
                return character == pState->m_commentDelimiter;
            }
        }

    }
}
