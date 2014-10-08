#pragma once

namespace lcpp
{
    /// The size of a memory page used by the garbage collector.
    enum { GarbageCollectorPageSize = 4096 };
}

// The following defines are only used for debugging.

/// Will always create a new survivor space instead of re-using the existing one.
#define LCPP_GC_AlwaysCreateNewSurvivor EZ_ON

/// Will collect after each allocation.
#define LCPP_GC_CollectAfterAllocation EZ_ON

/// Will never free allocated memory but leave it alive and (hopefully) protected.
#define LCPP_GC_KeepAllocatedMemory EZ_ON
