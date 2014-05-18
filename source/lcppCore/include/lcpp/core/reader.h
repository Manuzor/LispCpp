#pragma once
#include "lcpp/core/typeSystem.h"

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
        
    public:

        Reader();
        Reader(const CInfo& cinfo);
        ~Reader();

        SchemeObject& read(const ezString& inputString);

        SchemeInteger& parseInteger(const ezString& inputString);
        SchemeNumber&  parseNumber(const ezString& inputString);
        SchemeSymbol&  parseSymbol(const ezString& inputString);
        SchemeString&  parseString(const ezString& inputString);
        SchemeCons&    parseList(const ezString& inputString);

        void skipSeparators(ezStringIterator& iter);

        bool isSeparator(ezUInt32 character);

    private:

        ezString m_separators;

        TypeFactory m_defaultFactory;
        TypeFactory* m_pFactory;
    };
}
