#pragma once
#include "lcpp/core/memory/garbageCollectionCommon.h"
#include "lcpp/core/memory/stackPtr.h"
#include "lcpp/core/memory/fixedMemory.h"
#include "lcpp/core/typeSystem/type.h"

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

    class GarbageCollectionContext
    {
    public:

        /// \brief Convenience function for cleaner code
        template<typename T>
        EZ_FORCE_INLINE
        T* addSurvivor(T* pSurvivor)
        {
            return static_cast<T*>(addSurvivor(static_cast<CollectableBase*>(pSurvivor)));
        }

    protected:

        /// \brief Adds the given \a pObject as survivor, and returns the new address to it.
        /// \return \c nullptr if the operation failed.
        virtual CollectableBase* addSurvivor(CollectableBase* pSurvivor) = 0;

    };

    using DestructorFunction_t = void (*)(CollectableBase*);
    using PatchablePointerArray_t = ezDynamicArray<CollectableBase**>;
    using ScanFunction_t = void(*)(CollectableBase*, GarbageCollectionContext*);

    struct PreventCollection;

    class LCPP_API_CORE_CONT GarbageCollector : public GarbageCollectionContext
    {
        friend CollectableBase;
        friend PreventCollection;
    public:

        class CInfo
        {
        public:
            Ptr<ezAllocatorBase> m_pParentAllocator;

            /// Number of bytes for the used memory (eden) and the copying pool (survivor) EACH.
            ezUInt32 m_uiNumInitialPages;

            /// Threshold of range [0.0f, 1.0f] at which to start growing the number of pages used.
            float m_fGrowingThreshold;

            /// Maximum number of pages this garbage collector should be allocating at all.
            /// Keep in mind that this implementation needs 2 pools of equal size to live at the same time.
            /// Defaults to 2 GiB of memory allocated.
            ezUInt32 m_uiMaxPagesPerPool;

        public:
            CInfo();
        };

    public:

        GarbageCollector(GarbageCollector&&) = delete;
        GarbageCollector(const GarbageCollector&) = delete;
        void operator=(const GarbageCollector&) = delete;

        GarbageCollector();

        void initialize(const CInfo& cinfo);
        void clear();

        template<typename T>
        T* createStatic(Ptr<const MetaInfo> pMetaInfo);

        template<typename T>
        void destroyStatic(T* pObject);

        template<typename T>
        Ptr<T> create(Ptr<const MetaInfo> pMetaInfo);

        void collect(std::size_t uiNumMinBytesToFree);

        /// \remarks WARNING: Be very careful with this method!
        ///                   It cannot check if the pointer is truly pointing to a valid object.
        ///                   Use this only in controlled environment.
        /// \remarks It is mainly used for unit testing the garbage collector.
        bool isAlive(Ptr<CollectableBase> pCollectable) const;

        EZ_FORCE_INLINE bool isCollecting() const { return m_ScanPointer != nullptr; }
        //ezUInt32 getCurrentGeneration() const { return m_uiCurrentGeneration; }

        virtual CollectableBase* addSurvivor(CollectableBase* pSurvivor) override;

        template<typename T>
        void addRoot(T*& pCollectable);

        template<typename T>
        void removeRoot(T*& pCollectable);

        ezUInt32 getNumRoots() const { return m_roots.GetCount(); }

        void printStats();

    private:

        void prepareCollectionCycle();
        void finalizeCollectionCycle();

        bool isEdenObject(Ptr<CollectableBase> pObject) const;
        bool isSurvivorObject(Ptr<CollectableBase> pObject) const;

        /// \brief Goes through the survivors using the scan pointer.
        /// \return Number of survivors scanned.
        ezUInt64 scanSurvivorSpace();

        /// \brief Helper to keep the code cleaner.
        ScanFunction_t getScanFunction(CollectableBase* pObject);

        void addStackPointersToSurvivorSpace();

        void destroyGarbage();

        void addRootsToSurvivorSpace();

        void increaseNumCurrentPages();

    public:

        Ptr<ezAllocatorBase> m_pAllocator;

        mutable ezHybridArray<CollectableBase**, 16> m_roots;

        enum { NumMemoryPools = 2 };
        FixedMemory m_pools[NumMemoryPools];

        EZ_CHECK_AT_COMPILETIME_MSG(NumMemoryPools >= 2, "Need at least 2 memory pools.");

        mutable FixedMemory* m_pEdenSpace;
        mutable FixedMemory* m_pSurvivorSpace;

        float m_fGrowingThreshold;
        ezUInt32 m_uiNumCurrentPages;
        ezUInt32 m_uiMaxNumPages;
        ezUInt32 m_uiCurrentGeneration;
        byte_t* m_ScanPointer;
        bool m_bGrowBeforeNextCollection;
        ezUInt32 m_uiNumCollectionPreventions;

        std::size_t m_collectionStats[Type::ENUM_COUNT];
    };

    namespace detail
    {
        void assertObjectIsAlive(Ptr<CollectableBase> pCollectable);
        void assertObjectIsAlive(CollectableBase* pCollectable);
    }

    // TODO This function should be removed! Every LispObject that is created
    // should be created in the current LispRuntimeState context.
    LCPP_API_CORE_CONT GarbageCollector* getGarbageCollector();

    struct PreventCollection
    {
        PreventCollection()                      : m_pGC(getGarbageCollector())    { ++m_pGC->m_uiNumCollectionPreventions; }
        PreventCollection(GarbageCollector* pGC) : m_pGC(pGC)                      { ++m_pGC->m_uiNumCollectionPreventions; }

        ~PreventCollection()                                                       { --m_pGC->m_uiNumCollectionPreventions; m_pGC = nullptr; }

        GarbageCollector* m_pGC;
    };
}

#include "lcpp/core/memory/impl/garbageCollection.inl"

#define LCPP_AssertObjectIsAlive(pCollectable) ::lcpp::detail::assertObjectIsAlive(pCollectable)
#define LCPP_GC_PreventCollectionInScope ::lcpp::PreventCollection EZ_CONCAT(_PreventCollection_, EZ_SOURCE_LINE)
