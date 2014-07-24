#include "stdafx.h"
#include "lcpp/foundation/memory/allocator.h"

ezAllocatorBase* lcpp::defaultAllocator()
{
    static NonTrackingHeapAllocator allocator("lcpp/NonTrackingHeapAllocator");

    return &allocator;
}
