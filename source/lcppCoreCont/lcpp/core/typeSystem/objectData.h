#pragma once
#include "lcpp/core/memory/garbageCollection.h"

namespace lcpp
{
    class LCPP_API_CORE_CONT LispObject :
        public CollectableBase
    {
    public:

        template<typename T_Data>
        T_Data& getData();
    };
}

#include "lcpp/core/typeSystem/impl/objectData.inl"
