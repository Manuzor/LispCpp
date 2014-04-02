#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeSymbol :
        public SchemeExtend<SchemeSymbol, SchemeObject>
    {
    public:

        SchemeSymbol(const char* symbol);
        SchemeSymbol(const ezString& symbol);
        virtual ~SchemeSymbol();

        virtual bool operator ==(const SchemeObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        operator ezString() const;

        const ezString& value() const;
        void value(const ezString& newSymbol);
        void value(const char* newSymbol);

    private:
        ezString m_symbol;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeSymbol);

    bool operator ==(const SchemeSymbol& lhs, const SchemeSymbol& rhs);

    bool operator ==(const SchemeSymbol& lhs, const char* rhs);
    bool operator ==(const char* lhs, const SchemeSymbol& rhs);

    bool operator ==(const SchemeSymbol& lhs, const ezString& rhs);
    bool operator ==(const ezString& lhs, const SchemeSymbol& rhs);

}


#include "lcpp/core/typeSystem/types/implementation/symbol_inl.h"
