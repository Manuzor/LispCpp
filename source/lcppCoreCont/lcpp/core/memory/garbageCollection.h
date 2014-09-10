#pragma once
#include "lcpp/core/memory/refIndex.h"
#include "lcpp/core/memory/fixedMemory.h"

namespace lcpp
{
    class GarbageCollector;

    typedef unsigned char byte_t;

    class LCPP_API_CORE_CONT CollectableBase
    {
        friend GarbageCollector;
    public:

        /// \brief Gets the garbage collector associated with this collectable instance.
        Ptr<GarbageCollector> getGarbageCollector();
        Ptr<const GarbageCollector> getGarbageCollector() const;

        RefIndex getRefIndex() const;

    private:

        void setGarbageCollector(Ptr<GarbageCollector> pGarbageCollector);

        void setRefIndex(RefIndex refIndex);

    private:

        Ptr<GarbageCollector> m_pGarbageCollector;
        RefIndex m_refIndex;
    };

    class LCPP_API_CORE_CONT GarbageCollector
    {
        friend CollectableBase;
    public:

        GarbageCollector(Ptr<ezAllocatorBase> pParentAllocator);

        template<typename T>
        Ptr<T> createStatic();

        template<typename T>
        Ptr<T> create();

        template<typename T>
        T* getPointer(RefIndex refIndex) const;

        void collect();

    private:

        Ptr<ezAllocatorBase> m_pAllocator;

        mutable FixedMemory m_staticAllocations;
        mutable FixedMemory m_dynamicAllocations;
    };

    // TODO This function should be removed! Every LispObject that is created
    // should be created in the current LispRuntimeState context.
    LCPP_API_CORE_CONT GarbageCollector* getGarbageCollector();
}

#include "lcpp/core/memory/impl/garbageCollection.inl"
