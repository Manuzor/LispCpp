#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeString:
        public SchemeExtend<SchemeString, SchemeObject>
    {
    public:

        SchemeString(const char* str);
        SchemeString(const ezString& str);
        virtual ~SchemeString();

        virtual bool operator ==(const SchemeObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        operator ezString() const;

        const ezString& value() const;
        void value(const ezString& newSymbol);
        void value(const char* newSymbol);

    private:
        ezString m_string;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeString);

    bool operator ==(const SchemeString& lhs, const SchemeString& rhs);

    bool operator ==(const SchemeString& lhs, const char* rhs);
    bool operator ==(const char* lhs, const SchemeString& rhs);

    bool operator ==(const SchemeString& lhs, const ezString& rhs);
    bool operator ==(const ezString& lhs, const SchemeString& rhs);

}


#include "lcpp/core/typeSystem/types/implementation/string.inl"
