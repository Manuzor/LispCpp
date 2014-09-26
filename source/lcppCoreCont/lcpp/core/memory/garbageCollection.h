#pragma once
#include "lcpp/core/memory/refIndex.h"
#include "lcpp/core/memory/fixedMemory.h"

namespace lcpp
{
    class MetaInfo;
    class GarbageCollector;

    typedef unsigned char byte_t;

    class LCPP_API_CORE_CONT CollectableBase
    {
        friend GarbageCollector;
    public:

        CollectableBase();

        /// \brief Gets the garbage collector associated with this collectable instance.
        Ptr<GarbageCollector> getGarbageCollector();
        Ptr<const GarbageCollector> getGarbageCollector() const;

        Ptr<const MetaInfo> getMetaInfo() const;

    private:

        void setGarbageCollector(Ptr<GarbageCollector> pGarbageCollector);
        void setMetaInfo(Ptr<const MetaInfo> pMetaInfo);

    private:

        bool m_bIsForwarded;

        union
        {
            struct
            {
                GarbageCollector* m_pGarbageCollector;
                const MetaInfo* m_pMetaInfo;
                ezUInt64 m_uiMemorySize;
            };
            CollectableBase* m_pForwardPointer;
        };
    };

    using DestructorFunction_t = void (*)(Ptr<CollectableBase>);

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

        using PatchablePointerArray = ezHybridArray<Ptr<CollectableBase>*, 8>;
        using ScanFunction = void(*)(Ptr<CollectableBase>, PatchablePointerArray&);

    public:

        GarbageCollector(const CInfo& cinfo);

        template<typename T>
        Ptr<T> createStatic(Ptr<const MetaInfo> pMetaInfo);

        template<typename T>
        Ptr<T> create(Ptr<const MetaInfo> pMetaInfo);

        void collect();

        void addRoot(Ptr<CollectableBase> pCollectable);
        void removeRoot(Ptr<CollectableBase> pCollectable);
        bool isRoot(Ptr<CollectableBase> pCollectable) const;
        bool isAlive(Ptr<CollectableBase> pCollectable) const;

    private:

        bool isValidEdenPtr(Ptr<CollectableBase> pObject) const;

        void scanAndPatch(CollectableBase* pObject);

        void addSurvivor(CollectableBase* pSurvivor);

    private:

        Ptr<ezAllocatorBase> m_pAllocator;

        ezHybridArray<CollectableBase*, 64> m_roots;

        mutable FixedMemory m_edenSpace;
        mutable FixedMemory m_survivorSpace;
    };

    // TODO This function should be removed! Every LispObject that is created
    // should be created in the current LispRuntimeState context.
    LCPP_API_CORE_CONT GarbageCollector* getGarbageCollector();
}

#include "lcpp/core/memory/impl/garbageCollection.inl"
