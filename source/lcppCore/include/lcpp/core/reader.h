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

        SchemeObject* read(const ezString& input);
        SchemeObject* read(ezStringIterator& input);

        void skipSeparators(ezStringIterator& iter);

        bool isSeparator(ezUInt32 character);

    private:

        ezString m_separators;

        TypeFactory m_factoryDefault;
        TypeFactory* m_pFactory;
    };
}
