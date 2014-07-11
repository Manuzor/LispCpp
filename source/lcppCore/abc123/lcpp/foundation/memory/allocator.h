#pragma once

namespace lcpp
{
    LCPP_CORE_API ezAllocatorBase* defaultAllocator();

    typedef ezAllocator<ezMemoryPolicies::ezHeapAllocation, (ezMemoryTrackingFlags::Enum)0> NonTrackingHeapAllocator;
}
