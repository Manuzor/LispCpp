#pragma once
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/sourceCursor.h"

namespace lcpp
{
    class SchemeRuntime;
    class SchemeObject;

    class LCPP_CORE_API Reader
    {
        friend class SchemeRuntime;
    public:

        struct CInfo 
        {
            const char* separators;

            CInfo() :
                separators(" \t\r\n\v\f\a")
            {
            }
        };
        
        struct SyntaxCheckResult
        {
            bool valid;
            bool hasParenthesis;
            bool isPureWhitespace;
            ezInt32 parenthesisBalance;
            SourceCursor cursor;
            ezString info;

            inline SyntaxCheckResult() { reset(); }

            inline bool isComplete() const { return parenthesisBalance == 0; }
            inline void reset()
            {
                valid = false;
                hasParenthesis = false;
                isPureWhitespace = true;
                parenthesisBalance = 0;
                cursor.reset();
                info.Clear();
            }
        };

    public:

        ~Reader();

        void initialize();

        Ptr<SchemeObject> read(const ezString& inputString, bool resetSyntaxChecker = true);
        Ptr<SchemeObject> read(ezStringIterator& input, bool resetSyntaxChecker = true);

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

        inline       Ptr<SyntaxCheckResult> syntaxCheckResult() { return m_pSyntaxCheckResult; }
        inline Ptr<const SyntaxCheckResult> syntaxCheckResult() const { return m_pSyntaxCheckResult; }

    private:

        struct Defaults
        {
            SyntaxCheckResult syntaxCheckResult;
        };

    private:
        Ptr<SchemeRuntime> m_pRuntime;
        
        Defaults m_defaults;

        Ptr<SyntaxCheckResult> m_pSyntaxCheckResult;
        ezString m_separators;

        ezHashTable<ezString, SchemeSyntax_Builtin::HandlerFuncPtr_t> m_syntaxHandlers;

    private:
        explicit Reader(Ptr<SchemeRuntime> pRuntime, const CInfo& cinfo);

        ezUInt8 advance(ezStringIterator& iter);
        Ptr<SchemeObject> parseListHelper(ezStringIterator& input);
    };
}
