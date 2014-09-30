#pragma once
#include "lcpp/core/readerState.h"

namespace lcpp
{
    class LispRuntimeState;

    namespace reader
    {
        /// \brief Reads a given stream for lisp objects.
        /// \remark Continuation function.
        /// Expects the following arguments:
        /// [0]: stream from which to read from.
        LCPP_API_CORE_CONT StackPtr<LispObject> read(StackPtr<LispObject> pCont);

        /// \brief Defines a new character macro.
        /// \a pCharacter must be a symbol object which only has 1 character.
        LCPP_API_CORE_CONT void addCharacterMacro(Ptr<LispRuntimeState> pState, StackPtr<LispObject> pCharacter, StackPtr<LispObject> pLambda);

        LCPP_API_CORE_CONT void addSyntax(Ptr<LispRuntimeState> pState, StackPtr<LispObject> pSymbol, StackPtr<LispObject> pSyntax);
        LCPP_API_CORE_CONT ezResult getSyntax(Ptr<LispRuntimeState> pState, StackPtr<LispObject> pSymbol, StackPtr<LispObject>& out_pSyntax);

        namespace detail
        {
            LCPP_API_CORE_CONT StackPtr<LispObject> readAtom(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> readSymbol(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> readString(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> readList(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> readList_helper(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> readList_parsedCar(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> readList_finalize(StackPtr<LispObject> pCont);

            //////////////////////////////////////////////////////////////////////////
            
            LCPP_API_CORE_CONT ezUInt32 skipSeparators(Ptr<State> pState, StackPtr<LispObject> pStream);
            LCPP_API_CORE_CONT ezUInt32 skipToFirstNewLine(Ptr<State> pState, StackPtr<LispObject> pStream);

            /// \return The number of characters skipped over, which is currently constant at 1.
            LCPP_API_CORE_CONT ezUInt32 advance(Ptr<State> pState, StackPtr<LispObject> pStream);

            LCPP_API_CORE_CONT bool isSeparator(Ptr<State> pState, ezUInt32 character);
            LCPP_API_CORE_CONT bool isNewLine(Ptr<State> pState, ezUInt32 character);
            LCPP_API_CORE_CONT bool isSymbolDelimiter(Ptr<State> pState, ezUInt32 character);
            LCPP_API_CORE_CONT bool isCommentDelimiter(Ptr<State> pState, ezUInt32 character);

        }
    }
}
