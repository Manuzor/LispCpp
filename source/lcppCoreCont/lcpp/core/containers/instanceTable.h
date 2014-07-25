#pragma once

namespace lcpp
{
    class LispRuntime;

    template<typename T_Key, typename T_Value>
    class InsanceTable
    {
    public:
        static Ptr<InsanceTable> create();

    public:

        Ptr<T_Value> get(const T_Key& key);

    private:

        ezHashTable<T_Key, Ptr<T_Value>> m_table;

        InsanceTable();
    };
}

#include "lcpp/core/containers/impl/instanceTable.inl"
