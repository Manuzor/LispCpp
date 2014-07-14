#pragma once

namespace lcpp
{
    template<typename T_Key, typename T_Value>
    struct InsanceTable
    {
        InsanceTable(Ptr<LispRuntime> pRuntime);

        Ptr<T_Value> get(const T_Key& key);

    private:
        ezMap<T_Key, Ptr<T_Value>> m_table;
    };
}

#include "lcpp/core/implementation/instancing_inl.h"
