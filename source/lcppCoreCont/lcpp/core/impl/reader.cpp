#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/foundation/stringUtils.h"

namespace lcpp
{
    namespace reader
    {

        Ptr<LispObject> read(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            LCPP_NOT_IMPLEMENTED;
        }

        namespace detail
        {
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
