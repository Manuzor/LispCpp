#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeSymbol :
        public SchemeExtend<SchemeSymbol, SchemeObject>
    {
        friend class TypeFactory;
    public:

        virtual ~SchemeSymbol();

        virtual bool operator ==(const SchemeObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        const ezString& value() const;

    private:
        const ezString m_symbol;

        SchemeSymbol(const char* symbol);
        SchemeSymbol(const ezString& symbol);

        LCPP_DISALLOW_COPY_ASSIGNMENT(SchemeSymbol);
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeSymbol);

    bool operator ==(const SchemeSymbol& lhs, const SchemeSymbol& rhs);

}


#include "lcpp/core/typeSystem/types/implementation/symbol_inl.h"
