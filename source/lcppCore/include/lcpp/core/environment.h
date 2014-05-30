#pragma once

namespace lcpp
{
    class SchemeObject;
    class SchemeSymbol;

    class Environment
    {
    public:

        static Environment createTopLevelInstance();

        Environment(const ezString& name, Ptr<Environment> pParent);

        void add(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject> pValue);
        ezResult set(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject> pValue);
        ezResult get(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject>& out_value);

        bool exists(Ptr<SchemeSymbol> pKey);

        const ezString& name() const;
        ezString& name();
        ezString qualifiedName() const;

    private:
        Ptr<Environment> m_pParent;
        ezString m_name;
        ezHashTable<SchemeSymbol*, SchemeObject*> m_symbols;

        Environment();

        void qualifiedNameHelper(ezStringBuilder& builder) const;
    };


}

#include "lcpp/core/implementation/environment.inl"
