#pragma once

namespace lcpp
{
    LCPP_API_FOUNDATION ezAllocatorBase* defaultAllocator();

    enum { NonTrackingHeapAllocatorFlags = (ezMemoryTrackingFlags::Enum)0 };
    typedef ezAllocator<ezMemoryPolicies::ezHeapAllocation, NonTrackingHeapAllocatorFlags> NonTrackingHeapAllocator;

    struct AllocatorWrapper_Default
    {
        static ezAllocatorBase* GetAllocator();
    };

    class LCPP_API_FOUNDATION ElectricFenceAllocator : public ezAllocatorBase
    {
        struct AllocationInfo
        {
            EZ_DECLARE_POD_TYPE();

            void* ptr;
            size_t size;
        };

        ezDeque<AllocationInfo> m_freeQueue;
        ezMap<void*, AllocationInfo> m_allocationMap;

        static const size_t s_pageSize = 4096;
        static const size_t s_maxFreeQueueElements = 4096;

    public:
        ElectricFenceAllocator(ezAllocatorBase* pInternalContainerAllocator);

        virtual void* Allocate(size_t uiSize, size_t uiAlign) override;
        virtual void Deallocate(void* ptr) override;
        virtual size_t AllocatedSize(const void* ptr) override;
        virtual Stats GetStats() const override;

    };
}

#include "lcpp/foundation/memory/implementation/allocator.inl"
