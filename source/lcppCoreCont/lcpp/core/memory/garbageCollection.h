#pragma once
#include "lcpp/core/typeSystem/objectData.h"

namespace lcpp
{
    template<>
    struct Ptr<LispObject>
    {
        // Default construct as nullptr
        Ptr();

        // Copy constructor
        Ptr(const Ptr& rhs);

        // Move constructor
        Ptr(Ptr&& rhs);

        // Construct from raw pointer
        Ptr(LispObject* pPtr);

        ~Ptr();

        void operator =(const Ptr& toCopy);
        void operator =(Ptr&& toMove);
        void operator =(LispObject* pPtr);

        LispObject* operator ->() const;
        LispObject& operator *() const;

        LispObject* get() const;
        bool isNull() const;

        operator bool() const;

    private:
        LispObject* m_pPtr;
    };

    typedef unsigned char byte_t;

    class LCPP_API_CORE_CONT GarbageCollector :
        ezAllocatorBase
    {
    public:

        GarbageCollector(ezAllocatorBase* pParentAllocator);

        void collect();

        ezAllocatorBase* getAllocator();

    private:

        struct DualArrayWrapper
        {
            ezDynamicArray<byte_t> m_left;
            ezDynamicArray<byte_t> m_right;

            DualArrayWrapper(ezAllocatorBase* pParentAllocator);

            void SetCountUninitialized(ezUInt32 uiCount);
            void AddCountUninitialized(ezUInt32 uiCount);
            ezUInt32 GetCount();
            
            void EnsureRangeIsValid(ezUInt32 uiStartIndex, ezUInt32 uiCount);
        };

    private: // ezAllocatorBase interface

        virtual void* Allocate(size_t uiSize, size_t uiAlign) override;

        virtual void Deallocate(void* ptr) override;

        virtual size_t AllocatedSize(const void* ptr) override;

        virtual Stats GetStats() const override;

    private:

        ezAllocatorId m_id;
        ezAllocatorBase* m_pParent;
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
