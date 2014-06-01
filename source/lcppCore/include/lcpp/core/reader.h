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
            bool isPureWhitespace;
            ezInt32 parenthesisBalance;
            Ptr<SourceCursor> pCursor;
            ezString info;

            SyntaxCheckResult() :
                valid(false),
                hasParenthesis(false),
                isPureWhitespace(true),
                parenthesisBalance(0),
                pCursor(),
                info()
            {
            }

            inline bool isComplete() { return parenthesisBalance == 0; }
        };
        
    public:

        Reader();
        explicit Reader(const CInfo& cinfo);
        ~Reader();

        Ptr<SchemeObject> read(const ezString& inputString, bool resetCursor = true);
        Ptr<SchemeObject> read(ezStringIterator& input, bool resetCursor = true);

        Ptr<SchemeObject> parseAtom(ezStringIterator& input);

        /// \brief Tries to parse an integer from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<SchemeInteger> parseInteger(ezStringIterator& input);

        /// \brief Tries to parse a number from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<SchemeNumber>  parseNumber(ezStringIterator& input);

        /// \brief Tries to parse a symbol from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<SchemeSymbol>  parseSymbol(ezStringIterator& input);

        /// \brief Tries to parse a string from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<SchemeString>  parseString(ezStringIterator& input);

        /// \brief Tries to parse a list from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<SchemeObject>  parseList(ezStringIterator& input);

        /// \brief Checks for correctness of parenthesis
        /// \remark Always resets the cursor.
        SyntaxCheckResult checkBasicSyntax(const ezStringIterator& input);

        ezUInt32 skipSeparators(ezStringIterator& iter);
        /// \brief
        /// \remark Does not consume the encountered new-line character
        void skipToFirstNewLine(ezStringIterator& iter);

        bool isSeparator(ezUInt32 character);
        bool isNewLine(ezUInt32 character);
        bool isComment(ezUInt32 character);

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
        Ptr<SchemeObject> parseListHelper(ezStringIterator& input);
    };
}
