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
}

#include "lcpp/foundation/memory/implementation/allocator.inl"
