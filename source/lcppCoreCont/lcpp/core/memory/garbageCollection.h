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

    enum class GarbageState
    {
        Initial,
        Alive,
        Forwarded,
        Destroying,
        Garbage
    };

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

        bool isInitial() const    { return m_state == GarbageState::Initial; }
        bool isAlive() const      { return m_state == GarbageState::Alive; }
        bool isForwarded() const  { return m_state == GarbageState::Forwarded; }
        bool isDestroying() const { return m_state == GarbageState::Destroying; }
        bool isGarbage() const    { return m_state == GarbageState::Garbage; }

    private:

        GarbageState m_state;

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

    using DestructorFunction_t = void (*)(CollectableBase*);
    using PatchablePointerArray_t = ezHybridArray<CollectableBase**, 32>;
    using ScanFunction_t = void(*)(CollectableBase*, PatchablePointerArray_t&);

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

        void initialize(const CInfo& cinfo);
        void clear();

        template<typename T>
        Ptr<T> createStatic(Ptr<const MetaInfo> pMetaInfo);

        template<typename T>
        StackPtr<T> create(Ptr<const MetaInfo> pMetaInfo);

        void collect();

        void addRoot(Ptr<CollectableBase> pCollectable);
        void removeRoot(Ptr<CollectableBase> pCollectable);
        bool isRoot(Ptr<CollectableBase> pCollectable) const;

        /// \remarks WARNING: Be very careful with this method!
        ///                   It cannot check if the pointer is truly pointing to a valid object.
        ///                   Use this only in controlled environment.
        /// \remarks It is mainly used for unit testing the garbage collector.
        bool isAlive(Ptr<CollectableBase> pCollectable) const;

        void addStackPtr(const StackPtrBase* stackPtr) const;
        void removeStackPtr(const StackPtrBase* stackPtr) const;
        bool isOnStack(Ptr<CollectableBase> pCollectable) const;
        ezUInt32 getNumStackReferences() const { return m_stackReferences.GetCount(); }

        //bool isCollecting() const { return m_bIsCollecting; }
        //ezUInt32 getCurrentGeneration() const { return m_uiCurrentGeneration; }

    private:

        bool isEdenObject(Ptr<CollectableBase> pObject) const;
        bool isSurvivorObject(Ptr<CollectableBase> pObject) const;

        void scanAndPatch(CollectableBase* pObject);

        AllocatorResult addSurvivor(CollectableBase* pSurvivor);

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
