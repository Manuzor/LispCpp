#pragma once
#include "lcpp/core/memory/stackPtr.h"
#include "lcpp/core/memory/fixedMemory.h"

namespace lcpp
{
    class MetaInfo;
    class GarbageCollector;
    class CollectableBase;
    class StackPtrBase;

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
                size_t m_uiMemorySize;
                ezUInt32 m_uiGeneration;
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

        using PatchablePointerArray = ezHybridArray<CollectableBase**, 32>;
        using ScanFunction = void(*)(CollectableBase*, PatchablePointerArray&);

    public:

        GarbageCollector(const CInfo& cinfo);

        template<typename T>
        Ptr<T> createStatic(Ptr<const MetaInfo> pMetaInfo);

        template<typename T>
        StackPtr<T> create(Ptr<const MetaInfo> pMetaInfo);

        void collect();

        void addRoot(Ptr<CollectableBase> pCollectable);
        void removeRoot(Ptr<CollectableBase> pCollectable);
        bool isRoot(Ptr<CollectableBase> pCollectable) const;
        bool isAlive(Ptr<CollectableBase> pCollectable) const;

        void addStackPtr(const StackPtrBase* stackPtr) const;
        void removeStackPtr(const StackPtrBase* stackPtr) const;
        bool isOnStack(Ptr<CollectableBase> pCollectable) const;
        ezUInt32 getNumStackReferences() const { return m_stackReferences.GetCount(); }

        //bool isCollecting() const { return m_bIsCollecting; }
        //ezUInt32 getCurrentGeneration() const { return m_uiCurrentGeneration; }

    private:

        bool isValidEdenPtr(Ptr<CollectableBase> pObject) const;

        void scanAndPatch(CollectableBase* pObject);

        void addSurvivor(CollectableBase* pSurvivor);

    private:

        Ptr<ezAllocatorBase> m_pAllocator;

        ezHybridArray<CollectableBase*, 16> m_roots;
        mutable ezHybridArray<const StackPtrBase*, 128> m_stackReferences;

        mutable FixedMemory m_edenSpace;
        mutable FixedMemory m_survivorSpace;

        ezUInt32 m_uiCurrentGeneration;
        bool m_bIsCollecting;
    };

    namespace detail
    {
        void assertObjectIsAlive(Ptr<CollectableBase> pCollectable);
        void assertObjectIsAlive(CollectableBase* pCollectable);
    }

    // TODO This function should be removed! Every LispObject that is created
    // should be created in the current LispRuntimeState context.
    LCPP_API_CORE_CONT GarbageCollector* getGarbageCollector();
}

#include "lcpp/core/memory/impl/garbageCollection.inl"

#define LCPP_AssertObjectIsAlive(pCollectable) ::lcpp::detail::assertObjectIsAlive(pCollectable)
