﻿#pragma once
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/sourceCursor.h"

namespace lcpp
{
    class LispRuntime;
    class LispObject;

    class LCPP_API_CORE Reader
    {
    public:

        struct CInfo 
        {
            const char* separators;
            const char* symbolDelimiters;

            CInfo() :
                separators(" \t\r\n\v\f\a"),
                symbolDelimiters("()")
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
            String info;

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

        static Ptr<Reader> create(const CInfo& cinfo);

    public:

        ~Reader();

        void initialize();

        Ptr<LispObject> read(const String& inputString, bool resetSyntaxChecker = true);
        Ptr<LispObject> read(ezStringIterator& input, bool resetSyntaxChecker = true);

        Ptr<LispObject> parseAtom(ezStringIterator& input);

        /// \brief Tries to parse an integer from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<LispInteger> parseInteger(ezStringIterator& input);

        /// \brief Tries to parse a number from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<LispNumber>  parseNumber(ezStringIterator& input);

        /// \brief Tries to parse a symbol from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<LispSymbol>  parseSymbol(ezStringIterator& input);

        /// \brief Tries to parse a string from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<LispString>  parseString(ezStringIterator& input);

        /// \brief Tries to parse a list from the string iterator.
        /// \remark Does not reset the cursor.
        Ptr<LispObject>  parseList(ezStringIterator& input);

        /// \brief Checks for correctness of parenthesis
        /// \remark Always resets the cursor.
        SyntaxCheckResult checkBasicSyntax(const ezStringIterator& input);

        ezUInt32 skipSeparators(ezStringIterator& iter);
        /// \brief
        /// \remark Does not consume the encountered new-line character
        void skipToFirstNewLine(ezStringIterator& iter);

        bool isSeparator(ezUInt32 character);
        bool isSymbolDelimiter(ezUInt32 character);
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
        Defaults m_defaults;

        Ptr<SyntaxCheckResult> m_pSyntaxCheckResult;
        String m_separators;
        String m_symbolDelimiters;

    private:
        explicit Reader(const CInfo& cinfo);

        ezUInt8 advance(ezStringIterator& iter);
        Ptr<LispObject> parseListHelper(ezStringIterator& input);

        template<typename T_Object>
        Ptr<T_Object> create();

        template<typename T_Object, typename T_Arg0>
        Ptr<T_Object> create(T_Arg0 arg0);

        template<typename T_Object, typename T_Arg0, typename T_Arg1>
        Ptr<T_Object> create(T_Arg0, T_Arg1 arg1);
    };
}

#include "lcpp/core/implementation/reader.inl"
