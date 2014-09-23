#pragma once
#include "lcpp/core/memory/garbageCollection.h"
#include "lcpp/core/memory/refIndex.h"

namespace lcpp
{
    class LCPP_API_CORE_CONT LispObject :
        public CollectableBase
    {
    public:

        template<typename T_Data>
        T_Data& getData();

        /// \brief Do not use this member directly!
        void* m_data;
    };
}

#include "lcpp/core/typeSystem/impl/objectData.inl"
