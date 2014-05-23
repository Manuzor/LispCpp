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
            SourceCursor* pCursor;

            CInfo() :
                separators(" \t\r\n\v\f\a"),
                pFactory(nullptr),
                pCursor(nullptr)
            {
            }
        };

        struct SyntaxCheckResult
        {
            bool valid;
            bool hasParenthesis;
            ezInt32 parenthesisBalance;
            SourceCursor& cursor;
            ezString info;

            SyntaxCheckResult(SourceCursor& cursor) :
                valid(false),
                hasParenthesis(false),
                parenthesisBalance(0),
                cursor(cursor),
                info()
            {
            }

            inline bool isComplete() { return parenthesisBalance == 0; }
        };
        
    public:

        Reader();
        explicit Reader(const CInfo& cinfo);
        ~Reader();

        SchemeObject& read(const ezString& inputString, bool resetCursor = true);
        SchemeObject& read(ezStringIterator& input, bool resetCursor = true);

        /// \brief Tries to parse an integer from the string iterator.
        /// \remark Does not reset the cursor.
        SchemeInteger& parseInteger(ezStringIterator& input);

        /// \brief Tries to parse a number from the string iterator.
        /// \remark Does not reset the cursor.
        SchemeNumber&  parseNumber(ezStringIterator& input);

        /// \brief Tries to parse a symbol from the string iterator.
        /// \remark Does not reset the cursor.
        SchemeSymbol&  parseSymbol(ezStringIterator& input);

        /// \brief Tries to parse a string from the string iterator.
        /// \remark Does not reset the cursor.
        SchemeString&  parseString(ezStringIterator& input);

        /// \brief Tries to parse a list from the string iterator.
        /// \remark Does not reset the cursor.
        SchemeObject&  parseList(ezStringIterator& input);

        /// \brief Checks for correctness of parenthesis
        /// \remark Always resets the cursor.
        SyntaxCheckResult checkBasicSyntax(const ezStringIterator& input);

        ezUInt32 skipSeparators(ezStringIterator& iter);

        bool isSeparator(ezUInt32 character);
        bool isNewLine(ezUInt32 character);

        inline const SourceCursor& getCursor() const { return m_cursor; }
        inline SourceCursor& getCursor() { return m_cursor; }

    private:

        struct Defaults
        {
            TypeFactory factory;
            SourceCursor cursor;
        };

        Defaults defaults;

        ezString m_separators;

        TypeFactory& m_factory;
        SourceCursor& m_cursor;

        ezUInt8 advance(ezStringIterator& iter);
        SchemeObject& parseListHelper(ezStringIterator& input);
    };
}
