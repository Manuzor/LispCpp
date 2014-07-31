#pragma once
#include "lcpp/core/readerState.h"

namespace lcpp
{
    namespace reader
    {
        /// \brief Reads a given stream for lisp objects.
        /// \remark Continuation function.
        /// Expects the following arguments:
        /// [0]: stream from which to read from.
        LCPP_API_CORE_CONT Ptr<LispObject> read(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT Ptr<State> getState();
        LCPP_API_CORE_CONT void resetState();

        namespace detail
        {
            LCPP_API_CORE_CONT Ptr<LispObject> readAtom(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> readSymbol(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> readString(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> readList(Ptr<LispObject> pCont);

            //////////////////////////////////////////////////////////////////////////
            
            LCPP_API_CORE_CONT ezUInt32 skipSeparators(Ptr<State> pState, Ptr<LispObject> pStream);
            LCPP_API_CORE_CONT ezUInt32 skipToFirstNewLine(Ptr<State> pState, Ptr<LispObject> pStream);

            /// \return The number of characters skipped over, which is currently constant at 1.
            LCPP_API_CORE_CONT ezUInt32 advance(Ptr<State> pState, Ptr<LispObject> pStream);

            LCPP_API_CORE_CONT bool isSeparator(Ptr<State> pState, ezUInt32 character);
            LCPP_API_CORE_CONT bool isNewLine(Ptr<State> pState, ezUInt32 character);
            LCPP_API_CORE_CONT bool isSymbolDelimiter(Ptr<State> pState, ezUInt32 character);
            LCPP_API_CORE_CONT bool isCommentDelimiter(Ptr<State> pState, ezUInt32 character);

        }
    }
}
