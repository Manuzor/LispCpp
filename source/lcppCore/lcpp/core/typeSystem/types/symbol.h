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

        static Ptr<LispSymbol> create(const String& symbol);

        static const Type& typeInfo();

    private:

            /// \brief Creates a new instance without checking for an existing instance for the given \a value.
        static Ptr<LispSymbol> createNew(const String& value);

    public:

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;
        virtual const Type& type() const LCPP_OVERRIDE;
        virtual bool operator ==(const LispObject& rhs) const LCPP_OVERRIDE;
        virtual String toString() const LCPP_OVERRIDE;

        const String& value() const;

    private:
        const String m_symbol;

        LispSymbol(const char* symbol);
        LispSymbol(const String& symbol);

        LCPP_DISALLOW_COPY_ASSIGNMENT(LispSymbol);
    };

    bool operator ==(const LispSymbol& lhs, const LispSymbol& rhs);

}

#include "lcpp/core/typeSystem/types/implementation/symbol.inl"
