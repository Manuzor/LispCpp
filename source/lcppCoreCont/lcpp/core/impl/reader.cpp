#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/foundation/stringUtils.h"

#include "lcpp/core/exceptions/invalidInputException.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/objectUtils.h"

namespace lcpp
{
    namespace reader
    {

        Ptr<LispObject> read(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);
            auto pState = cont::getRuntimeState(pCont)->getReaderState();

            auto pStack = cont::getStack(pCont);
            auto pStream = pStack->get(0);
            typeCheck(pStream, Type::Stream);

            detail::skipSeparators(pState, pStream);

            if (!stream::isValid(pStream))
            {
                LCPP_cont_return(pCont, LCPP_pVoid);
            }

            auto pCharacter = symbol::create(stream::getCharacter(pStream));
            auto pCharacterHandler = LCPP_pNil;

            if(env::getBinding(pState->m_pMacroEnv, pCharacter, pCharacterHandler).Succeeded())
            {
                pStack->push(pCharacterHandler);
                LCPP_cont_tailCall(pCont, &call);
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

            if (!isCallable(pLambda))
            {
                LCPP_THROW(exceptions::InvalidInput("The object the character macro's character is bound to must be callable!"));
            }

            env::addBinding(pEnv, pCharacter, pLambda);
        }

        void addSyntax(Ptr<LispRuntimeState> pState, Ptr<LispObject> pSymbol, Ptr<LispObject> pSyntax)
        {
            typeCheck(pSymbol, Type::Symbol);
            typeCheck(pSymbol, Type::Syntax);

            env::addBinding(pState->getSyntaxEnvironment(), pSymbol, pSyntax);
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
                auto pState = cont::getRuntimeState(pCont)->getReaderState();

                auto pStream = cont::getStack(pCont)->get(0);
                typeCheck(pStream, Type::Stream);

                // Read the first " character
                advance(pState, pStream);

                auto theString = ezStringBuilder();

                auto ch = stream::getCharacter(pStream);
                while(stream::isValid(pStream) && ch != '"')
                {
                    theString.Append(ch);
                    advance(pState, pStream);
                    ch = stream::getCharacter(pStream);
                }

                LCPP_cont_return(pCont, str::create(theString));
            }

            Ptr<LispObject> readList(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                auto pState = cont::getRuntimeState(pCont)->getReaderState();

                auto pStream = pStack->get(0);
                typeCheck(pStream, Type::Stream);

                //////////////////////////////////////////////////////////////////////////

                skipSeparators(pState, pStream);

                EZ_ASSERT(stream::getCharacter(pStream) == '(', "Invalid input to readList.");

                // Read the '(' character.
                advance(pState, pStream);
                
                // Let the helper read the 'opened' list.
                LCPP_cont_tailCall(pCont, &readList_helper);
            }

            Ptr<LispObject> readList_helper(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                auto pState = cont::getRuntimeState(pCont)->getReaderState();

                auto pStream = pStack->get(0);
                typeCheck(pStream, Type::Stream);

                //////////////////////////////////////////////////////////////////////////

                skipSeparators(pState, pStream);

                if (!stream::isValid(pStream))
                {
                    LCPP_cont_return(pCont, LCPP_pVoid);
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

            Ptr<LispObject> readList_parsedCar(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                auto pState = cont::getRuntimeState(pCont);

                auto pStream = pStack->get(0);
                typeCheck(pStream, Type::Stream);

                //////////////////////////////////////////////////////////////////////////

                auto pCar = pStack->get(1);

                if(pCar->isType(Type::Symbol))
                {
                    auto pSyntax = LCPP_pNil;

                    if(env::getBinding(pState->getSyntaxEnvironment(), pCar, pSyntax).Succeeded())
                    {
                        // TODO call syntax handler.
                        LCPP_NOT_IMPLEMENTED;
                    }
                }

                if (!stream::isValid(pStream))
                {
                    LCPP_cont_return(pCont, LCPP_pVoid);
                }

                // Read cdr and then finalize the reading.
                cont::setFunction(pCont, &readList_finalize);
                LCPP_cont_call(pCont, &readList_helper, pStream);
            }

            Ptr<LispObject> readList_finalize(Ptr<LispObject> pCont)
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
