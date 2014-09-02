#pragma once
#include "lcpp/core/memory/refIndex.h"

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

    class LCPP_API_CORE_CONT GarbageCollector :
        private ezAllocatorBase
    {
        friend CollectableBase;
    public:

        GarbageCollector(ezAllocatorBase* pParentAllocator);
        ~GarbageCollector();

        template<typename T>
        Ptr<T> create();

        template<typename T>
        T* getPointer(RefIndex refIndex) const;

        void collect();

        ezAllocatorBase* getAllocator();

    private:

        struct DualArrayWrapper
        {
            struct Action
            {
                enum Enum
                {
                    None,
                    Resized,
                };

                Enum m_value;

                Action(Enum value);

                bool wasResized() const;
            };

            ezDynamicArray<byte_t> m_left;
            ezDynamicArray<byte_t> m_right;

            DualArrayWrapper(ezAllocatorBase* pParentAllocator);

            void SetCountUninitialized(ezUInt32 uiCount);
            void AddCountUninitialized(ezUInt32 uiCount);
            ezUInt32 GetCount();
            
            Action EnsureRangeIsValid(ezUInt32 uiStartIndex, ezUInt32 uiCount);
        };

    private: // ezAllocatorBase interface

        virtual void* Allocate(size_t uiSize, size_t uiAlign) override;

        virtual void Deallocate(void* ptr) override;

        virtual size_t AllocatedSize(const void* ptr) override;

        virtual Stats GetStats() const override;

    private:

        ezAllocatorId m_id;
        Stats m_stats;

        DualArrayWrapper m_data;

        ezDynamicArray<byte_t>* m_pEdenSpace;
        ezDynamicArray<byte_t>* m_pSurvivorSpace;

        ezUInt32 m_uiAllocationIndex;
    };

    // TODO This function should be removed! Every LispObject that is created
    // should be created in the current LispRuntimeState context.
    LCPP_API_CORE_CONT GarbageCollector* getGarbageCollector();
}

#include "lcpp/core/memory/impl/garbageCollection.inl"
