#pragma once

namespace lcpp
{
    struct Type;
    class LispSymbol;

    class LCPP_API_CORE NameableBase
    {
    public:

        static const Type& typeInfo();

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
