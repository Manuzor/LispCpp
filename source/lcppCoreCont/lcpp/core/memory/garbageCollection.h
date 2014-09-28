#pragma once
#include "lcpp/core/memory/refIndex.h"
#include "lcpp/core/memory/fixedMemory.h"

namespace lcpp
{
    class MetaInfo;
    class GarbageCollector;
    class CollectableBase;

    typedef unsigned char byte_t;

    class StackPtrBase
    {
        friend GarbageCollector;
    protected:
        mutable Ptr<CollectableBase> m_ptr;
    };

    template<typename T>
    class StackPtr : public StackPtrBase
    {
        EZ_CHECK_AT_COMPILETIME_MSG((std::is_convertible<T, CollectableBase>::value), "T needs to be a collectable object!");
    public:
        // Default construct as nullptr
        StackPtr();

        // Copy constructor
        StackPtr(const StackPtr& rhs);

        // Move constructor
        StackPtr(StackPtr&& rhs);

        // Construct from Ptr
        StackPtr(Ptr<T> ptr);

        // Construct from other StackPtr<>
        template<typename T_Other>
        StackPtr(const StackPtr<T_Other>& other);

        ~StackPtr();

        void operator =(const StackPtr& toCopy);
        void operator =(StackPtr&& toMove);
        void operator =(Ptr<T> ptr);

        T* operator ->() const;
        T& operator *() const;

        T* get() const;
        bool isNull() const;

        template<typename T_Other>
        StackPtr<T_Other> cast() const;

        operator bool() const;

    private:
        void addToGc() const;
        void removeFromGc() const;
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

    private:

        bool m_bIsForwarded;

        union
        {
            struct
            {
                GarbageCollector* m_pGarbageCollector;
                const MetaInfo* m_pMetaInfo;
                size_t m_uiMemorySize;
                mutable ezUInt64 m_uiStackReferenceCount;
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

        void addStackPtr(const StackPtrBase* stackPtr) const;
        void removeStackPtr(const StackPtrBase* stackPtr) const;
        bool isOnStack(Ptr<CollectableBase> pCollectable) const;

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
    };

    // TODO This function should be removed! Every LispObject that is created
    // should be created in the current LispRuntimeState context.
    LCPP_API_CORE_CONT GarbageCollector* getGarbageCollector();
}

#include "lcpp/core/memory/impl/garbageCollection.inl"
