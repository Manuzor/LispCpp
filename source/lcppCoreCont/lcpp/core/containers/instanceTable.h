#pragma once

namespace lcpp
{
    class LispRuntimeState;
    class LispObject;

    template<typename T_Key>
    class InsanceTable
    {
    public:

        typedef Ptr<LispObject>(*createNew_t)(const T_Key& value);

    public:

        InsanceTable(createNew_t pFunctor_createNew);

        Ptr<LispObject> get(const T_Key& key);

        ezHashTable<T_Key, LispObject*>& getTable() { return m_table; }

    private:

        createNew_t m_pFunctor_createNew;

        ezHashTable<T_Key, LispObject*> m_table;

    };
}

#include "lcpp/core/containers/impl/instanceTable.inl"
