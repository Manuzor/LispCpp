#pragma once

namespace lcpp
{
    class LispRuntimeState;
    class LispObject;

    template<typename T_Key>
    class InsanceTable
    {
    public:

        typedef StackPtr<LispObject>(*createNew_t)(const T_Key& value);

    public:

        InsanceTable(createNew_t pFunctor_createNew);

        StackPtr<LispObject> get(const T_Key& key);

    private:

        createNew_t m_pFunctor_createNew;

        ezHashTable<T_Key, LispObject*> m_table;

    };
}

#include "lcpp/core/containers/impl/instanceTable.inl"
