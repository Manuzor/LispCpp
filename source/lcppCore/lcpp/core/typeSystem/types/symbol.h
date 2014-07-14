#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/instancing.h"

namespace lcpp
{
    class LCPP_CORE_API LispSymbol :
        public LispObject
    {
        friend InstanceTable_Symbols;
    public:

        static Ptr<LispSymbol> create(const ezString& symbol);

        static const Type& typeInfo();

    private:

            /// \brief Creates a new instance without checking for an existing instance for the given \a value.
        static Ptr<LispSymbol> createNew(const ezString& value);

    public:

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;
        virtual const Type& type() const LCPP_OVERRIDE;
        virtual bool operator ==(const LispObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        const ezString& value() const;

    private:
        const ezString m_symbol;

        LispSymbol(const char* symbol);
        LispSymbol(const ezString& symbol);

        LCPP_DISALLOW_COPY_ASSIGNMENT(LispSymbol);
    };

    bool operator ==(const LispSymbol& lhs, const LispSymbol& rhs);

}

#include "lcpp/core/typeSystem/types/implementation/symbol_inl.h"
