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

    private:

        void setGarbageCollector(Ptr<GarbageCollector> pGarbageCollector);

    private:

        Ptr<GarbageCollector> m_pGarbageCollector;
    };

    class LCPP_API_CORE_CONT GarbageCollector
    {
        friend CollectableBase;
    public:

        class CInfo
        {
        public:
            Ptr<ezAllocatorBase> m_pParentAllocator;
            ezUInt32 m_uiInitialMemoryLimit; ///< Number of bytes for the used memory (eden) and the copying pool (survivor) EACH.

        public:
            CInfo();
        };

    public:

        GarbageCollector(const CInfo& cinfo);

        template<typename T>
        Ptr<T> createStatic();

        template<typename T>
        Ptr<T> create();

        void collect();

    private:

        Ptr<ezAllocatorBase> m_pAllocator;

        mutable FixedMemory m_edenSpace;
        mutable FixedMemory m_survivorSpace;
    };

    // TODO This function should be removed! Every LispObject that is created
    // should be created in the current LispRuntimeState context.
    LCPP_API_CORE_CONT GarbageCollector* getGarbageCollector();
}

#include "lcpp/core/memory/impl/garbageCollection.inl"
