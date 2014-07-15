#pragma once
#include "lcpp/core/typeSystem/types/numberDeclarations.h"

namespace lcpp
{
    class LispRuntime;
    class LispSymbol;
    class LispInteger;

    template<typename T_Key, typename T_Value>
    struct InsanceTable
    {
        InsanceTable();

        Ptr<T_Value> get(const T_Key& key);

    private:
        ezHashTable<T_Key, Ptr<T_Value>> m_table;
    };

    typedef InsanceTable<ezString, LispSymbol>         InstanceTable_Symbols;
    typedef InsanceTable<LispIntegerType, LispInteger> InstanceTable_Integers;
}

#include "lcpp/core/implementation/instancing_inl.h"