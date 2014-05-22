#pragma once
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/sourceCursor.h"

namespace lcpp
{
    class SchemeObject;

    class LCPP_CORE_API Reader
    {
    public:
        struct CInfo 
        {
            const char* separators;
            TypeFactory* pFactory;

            CInfo() :
                separators(" \t\r\n\v\f\a"),
                pFactory(nullptr)
            {
            }
        };

        struct SyntaxCheckResult
        {
            bool valid;
            bool hasParenthesis;
            ezInt32 parenthesisBalance;
            SourceCursor cursor;
            ezString info;

            SyntaxCheckResult() :
                valid(false),
                hasParenthesis(false),
                parenthesisBalance(0),
                cursor(SourceCursor::beginning()),
                info()
            {
            }
        };
        
    public:

        Reader();
        explicit Reader(const CInfo& cinfo);
        ~Reader();

        SchemeObject& read(const ezString& inputString);
        SchemeObject& read(ezStringIterator& input);

        SchemeInteger& parseInteger(ezStringIterator& input);
        SchemeNumber&  parseNumber(ezStringIterator& input);
        SchemeSymbol&  parseSymbol(ezStringIterator& input);
        SchemeString&  parseString(ezStringIterator& input);
        SchemeObject&  parseList(ezStringIterator& input);

        ezUInt32 skipSeparators(ezStringIterator& iter);

        bool isSeparator(ezUInt32 character);

        SyntaxCheckResult checkSyntax(const ezStringIterator& input);

    private:

        ezString m_separators;

        TypeFactory m_defaultFactory;
        TypeFactory* m_pFactory;

        SchemeObject& parseListHelper(ezStringIterator& input);
    };
}
