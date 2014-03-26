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

        virtual bool operator ==(const SchemeObject& rhs) const override;
        virtual ezString toString() const override;

        const ezString& symbol() const;
        void symbol(const ezString& newSymbol);
        void symbol(const char* newSymbol);

    private:
        ezString m_symbol;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeSymbol);

    bool operator ==(const SchemeSymbol& lhs, const SchemeSymbol& rhs);
}


#include "lcpp/core/typeSystem/types/implementation/symbol_inl.h"
