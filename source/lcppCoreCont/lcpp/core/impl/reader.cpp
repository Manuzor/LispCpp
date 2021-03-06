#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/foundation/stringUtils.h"

#include "lcpp/core/exceptions/invalidInputException.h"
#include "lcpp/core/exceptions/readerException.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/object.h"

#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/cons.h"

namespace lcpp
{
    namespace reader
    {
        static Ptr<reader::State> getReaderState(Ptr<LispObject> pCont)
        {
            return cont::getRuntimeState(pCont)->getReaderState();
        }

        Ptr<LispObject> read(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            auto pStack = cont::getStack(pCont);
            StackPtr<LispObject> pStream = pStack->get(0);
            typeCheck(pStream, Type::Stream);

            detail::skipSeparators(getReaderState(pCont), pStream);

            if (!stream::isValid(pStream))
            {
                LCPP_cont_return(pCont, LCPP_pVoid);
            }

            StackPtr<LispObject> pCharacter = symbol::create(stream::getCharacter(pStream));
            auto pCharacterHandler = LCPP_pNil;

            if(env::getBinding(getReaderState(pCont)->m_pMacroEnv, pCharacter, pCharacterHandler).Succeeded())
            {
                pStack->clear();
                pStack->push(getReaderState(pCont)->m_pMacroEnv);
                pStack->push(pStream);
                pStack->push(pCharacterHandler);
                LCPP_cont_tailCall(pCont, &object::call);
            }

            switch(stream::getCharacter(pStream))
            {
            case ')':
                throw exceptions::InvalidInput("Unexpected character ')'.");
            }

            LCPP_cont_tailCall(pCont, &detail::readAtom);
        }

        void addCharacterMacro(Ptr<LispRuntimeState> pState, Ptr<LispObject> pCharacter, Ptr<LispObject> pLambda)
        {
            LCPP_GC_PreventCollectionInScope;

            static auto emptyString = String();

            auto pEnv = pState->getReaderState()->m_pMacroEnv;

            auto& stringValue = symbol::getValue(pCharacter);

            if (stringValue.IsEmpty())
            {
                LCPP_THROW(exceptions::InvalidInput("Cannot add a character macro when only an empty character is given!"));
            }

            if (stringValue.GetCharacterCount() > 1)
            {
                LCPP_THROW(exceptions::InvalidInput("Cannot add a character macro when more than 1 character is given!"));
            }

            if(!object::isCallable(pLambda))
            {
                LCPP_THROW(exceptions::InvalidInput("The object the character macro's character is bound to must be callable!"));
            }

            env::addBinding(pEnv, pCharacter, pLambda);

            if(!object::hasName(pLambda))
            {
                object::setName(pLambda, pCharacter);
            }
        }

        void addSyntax(Ptr<LispRuntimeState> pState, Ptr<LispObject> pSymbol, Ptr<LispObject> pSyntax)
        {
            LCPP_GC_PreventCollectionInScope;

            typeCheck(pSymbol, Type::Symbol);
            typeCheck(pSyntax, Type::Syntax);

            env::addBinding(pState->getSyntaxEnvironment(), pSymbol, pSyntax);

            if(!object::hasName(pSyntax))
            {
                object::setName(pSyntax, pSymbol);
            }
        }

        ezResult getSyntax(Ptr<LispRuntimeState> pState, Ptr<LispObject> pSymbol, Ptr<LispObject>& out_pSyntax)
        {
            LCPP_GC_PreventCollectionInScope;

            Ptr<LispObject> pFetchResult;
            auto result = env::getBinding(pState->getSyntaxEnvironment(), pSymbol, pFetchResult);
            out_pSyntax = pFetchResult;
            return result;
        }

        namespace detail
        {
            Ptr<LispObject> readAtom(StackPtr<LispObject> pCont)
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
                    auto updateStreamPosition = [&]
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

                        updateStreamPosition();
                        LCPP_cont_return(pCont, number::create(theFloat));
                    }

                    updateStreamPosition();
                    LCPP_cont_return(pCont, number::create(integer));
                }


                LCPP_cont_tailCall(pCont, &readSymbol);
            }

            Ptr<LispObject> readSymbol(StackPtr<LispObject> pCont)
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

            Ptr<LispObject> readString(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pState = cont::getRuntimeState(pCont)->getReaderState();

                auto pStream = cont::getStack(pCont)->get(1);
                typeCheck(pStream, Type::Stream);

                auto ch = stream::getCharacter(pStream);

                if (ch != '"')
                {
                    pState->m_syntaxCheckResult.m_valid = false;
                    LCPP_THROW(exceptions::MissingStringDelimiter("Missing leading \" character in string."));
                }


                // Read the first " character
                advance(pState, pStream);

                auto theString = ezStringBuilder();

                ch = stream::getCharacter(pStream);
                while(ch != '"')
                {
                    if(!stream::isValid(pStream))
                    {
                        pState->m_syntaxCheckResult.m_valid = false;
                        LCPP_THROW(exceptions::MissingStringDelimiter("Missing trailing \" character in string."));
                    }

                    theString.Append(ch);
                    advance(pState, pStream);
                    ch = stream::getCharacter(pStream);
                }

                // Read the trailing " character.
                advance(pState, pStream);
                LCPP_cont_return(pCont, str::create(theString.GetData(), theString.GetElementCount()));
            }

            static Ptr<LispObject> quoteHelper(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                StackPtr<LispObject> pReadResult = cont::getStack(pCont)->get(-1);

                StackPtr<LispObject> pGlobalEnv = cont::getRuntimeState(pCont)->getGlobalEnvironment();

                StackPtr<LispObject> pQuote = LCPP_pNil;

                {
                    LCPP_GC_PreventCollectionInScope;

                    Ptr<LispObject> pRawQuote;
                    env::getBinding(pGlobalEnv, symbol::create("quote"), pRawQuote);
                    pQuote = pRawQuote;
                }

                // (quote <read-result>)
                StackPtr<LispObject> pQuoteCall = cons::create(pQuote,
                                                  cons::create(pReadResult,
                                                               LCPP_pNil));

                LCPP_cont_return(pCont, pQuoteCall);
            }

            Ptr<LispObject> readQuote(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto pStream = pStack->get(1);

                if (stream::getCharacter(pStream) != '\'')
                {
                    LCPP_THROW(exceptions::InvalidInput("Expected a ' character."));
                }

                stream::next(pStream);

                cont::setFunction(pCont, &quoteHelper);
                LCPP_cont_call(pCont, &read, pStream);
            }

            Ptr<LispObject> readList(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                auto pState = cont::getRuntimeState(pCont)->getReaderState();

                auto pStream = pStack->get(1);
                typeCheck(pStream, Type::Stream);

                //////////////////////////////////////////////////////////////////////////

                skipSeparators(pState, pStream);

                if(stream::getCharacter(pStream) != '(')
                {
                    LCPP_THROW(exceptions::MissingLeftListDelimiter("Missing leading ( character in list."));
                }

                // Read the '(' character.
                advance(pState, pStream);

                // Let the helper read the 'opened' list.
                LCPP_cont_tailCall(pCont, &readList_helper);
            }

            Ptr<LispObject> readList_helper(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                auto pState = cont::getRuntimeState(pCont)->getReaderState();

                auto pStream = pStack->get(1);
                typeCheck(pStream, Type::Stream);

                //////////////////////////////////////////////////////////////////////////

                skipSeparators(pState, pStream);

                if (!stream::isValid(pStream))
                {
                    LCPP_THROW(exceptions::MissingRightListDelimiter("Missing trailing ) character in list."));
                }

                if(stream::getCharacter(pStream) == ')')
                {
                    advance(pState, pStream);
                    LCPP_cont_return(pCont, LCPP_pNil);
                }

                // Read car and then process it.
                cont::setFunction(pCont, &readList_parsedCar);
                LCPP_cont_call(pCont, &read, pStream);
            }

            Ptr<LispObject> readList_parsedCar(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                auto pState = cont::getRuntimeState(pCont);

                auto pEnv = pStack->get(0);
                auto pStream = pStack->get(1);
                typeCheck(pStream, Type::Stream);

                //////////////////////////////////////////////////////////////////////////

                auto& pCar = pStack->get(2);

                if(object::isType(pCar, Type::Symbol))
                {
                    Ptr<LispObject> pSyntax = LCPP_pNil;

                    if(getSyntax(pState, pCar, pSyntax).Succeeded())
                    {
                        pCar = pSyntax;
                    }
                }

                if (!stream::isValid(pStream))
                {
                    LCPP_THROW(exceptions::MissingRightListDelimiter("Missing trailing ) character in list."));
                }

                // Read cdr and then finalize the reading.
                cont::setFunction(pCont, &readList_finalize);
                LCPP_cont_call(pCont, &readList_helper, pEnv, pStream);
            }

            Ptr<LispObject> readList_finalize(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                //////////////////////////////////////////////////////////////////////////

                auto pCar = pStack->get(-2);
                auto pCdr = pStack->get(-1);

                auto pCons = cons::create(pCar, pCdr);
                LCPP_cont_return(pCont, pCons);
            }

            //////////////////////////////////////////////////////////////////////////

            ezUInt32 skipSeparators(Ptr<State> pState, Ptr<LispObject> pStream)
            {
                LCPP_GC_PreventCollectionInScope;

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
                LCPP_GC_PreventCollectionInScope;

                typeCheck(pStream, Type::Stream);

                auto count = ezUInt32(0);
                auto& iter = stream::getIterator(pStream);

                while(!isNewLine(pState, iter.GetCharacter())
                   && stream::isValid(pStream))
                {
                    count += advance(pState, pStream);
                }

                return count;
            }

            ezUInt32 advance(Ptr<State> pState, Ptr<LispObject> pStream)
            {
                LCPP_GC_PreventCollectionInScope;

                typeCheck(pStream, Type::Stream);

                auto count = ezUInt32(1);
                auto character = stream::getCharacter(pStream);

                if(isNewLine(pState, character))
                {
                    pState->m_syntaxCheckResult.m_cursor.lineBreak();
                }
                else
                {
                    if (!isWhiteSpace(character))
                    {
                        pState->m_syntaxCheckResult.m_isPureWhitespace = false;
                    }

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
