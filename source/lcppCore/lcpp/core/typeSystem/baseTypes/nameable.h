#pragma once

namespace lcpp
{
    class LispSymbol;

    class LCPP_CORE_API NameableBase
    {
    public:

        explicit NameableBase(Ptr<LispSymbol> pName = Ptr<LispSymbol>());

        bool hasName() const;

        virtual void name(Ptr<LispSymbol> pNewName);
        Ptr<const LispSymbol> name() const;
        Ptr<LispSymbol> name();

    protected:
        Ptr<LispSymbol> m_pName;
    };
}

#include "lcpp/core/typeSystem/baseTypes/implementation/nameable.inl"
