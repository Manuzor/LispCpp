#pragma once

namespace lcpp
{
    namespace reader
    {

        LCPP_API_CORE_CONT Ptr<LispObject> read(Ptr<LispObject> pStream);

        //////////////////////////////////////////////////////////////////////////

        namespace detail
        {
            LCPP_API_CORE_CONT ezUInt32 skipSeparators(Ptr<LispObject> pStream);
            LCPP_API_CORE_CONT ezUInt32 skipToFirstNewLine(Ptr<LispObject> pStream);

            /// \return The number of characters skipped over, which is currently constant at 1.
            LCPP_API_CORE_CONT ezUInt32 advance(Ptr<LispObject> pStream);

            LCPP_API_CORE_CONT bool isSeparator(ezUInt32 character);
            LCPP_API_CORE_CONT bool isNewLine(ezUInt32 character);
            LCPP_API_CORE_CONT bool isSymbolDelimiter(ezUInt32 character);
            LCPP_API_CORE_CONT bool isCommentDelimiter(ezUInt32 character);

        }

    }
}
