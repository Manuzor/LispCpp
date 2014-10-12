#pragma once
#include "lcpp/core/memory/garbageCollectionCommon.h"
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

    class LCPP_API_CORE_CONT GarbageCollector : public GarbageCollectionContext
    {
        friend CollectableBase;
    public:

        class CInfo
        {
        public:
            Ptr<ezAllocatorBase> m_pParentAllocator;
            ezUInt32 m_uiNumInitialPages; ///< Number of bytes for the used memory (eden) and the copying pool (survivor) EACH.

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
        Ptr<T> create(Ptr<const MetaInfo> pMetaInfo);

        void collect();

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

    private:

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

    private:

        Ptr<ezAllocatorBase> m_pAllocator;

        mutable ezHybridArray<CollectableBase**, 16> m_roots;

        enum { NumMemoryPools = 2 };
        ezStaticArray<FixedMemory, NumMemoryPools> m_pools;

        EZ_CHECK_AT_COMPILETIME_MSG(NumMemoryPools >= 2, "Need at least 2 memory pools.");

        mutable FixedMemory* m_pEdenSpace;
        mutable FixedMemory* m_pSurvivorSpace;

        ezUInt32 m_uiNumCurrentPages;
        ezUInt32 m_uiCurrentGeneration;
        byte_t* m_ScanPointer;
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
