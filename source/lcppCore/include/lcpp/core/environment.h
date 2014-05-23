#pragma once

namespace lcpp
{
    class SchemeObject;
    class SchemeSymbol;

    class Environment
    {
    public:

        static Environment createTopLevelInstance();

        Environment(const ezString& name, Environment* pParent);

        void set(SchemeSymbol& key, SchemeObject& value);
        ezResult get(SchemeSymbol& key, SchemeObject*& out_value);

        bool exists(SchemeSymbol& key);

        const ezString& name() const;
        ezString& name();
        ezString qualifiedName() const;

    private:
        Environment* m_pParent;
        ezString m_name;
        ezHashTable<SchemeSymbol*, SchemeObject*> m_symbols;

        Environment();

        void qualifiedNameHelper(ezStringBuilder& builder) const;
    };


}

#include "lcpp/core/implementation/environment.inl"
