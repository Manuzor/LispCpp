#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispSymbol :
        public LispExtend<LispSymbol, LispObject>
    {
        friend class TypeFactory;
    public:

        virtual ~LispSymbol();

        virtual bool operator ==(const LispObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        const ezString& value() const;

    private:
        const ezString m_symbol;

        LispSymbol(const char* symbol);
        LispSymbol(const ezString& symbol);

        LCPP_DISALLOW_COPY_ASSIGNMENT(LispSymbol);
    };

    DECLARE_SCHEME_TYPE_INFO(LispSymbol);

    bool operator ==(const LispSymbol& lhs, const LispSymbol& rhs);

}


#include "lcpp/core/typeSystem/types/implementation/symbol_inl.h"
