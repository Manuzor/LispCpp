#pragma once

namespace lcpp
{
    /// The size of a memory page used by the garbage collector.
    enum { GarbageCollectorPageSize = 4096 };
}

// The following defines are only used for debugging.

/// Will always create a new survivor space instead of re-using the existing one.
/// \remark Beware that the garbage collector will leak memory when using this option.
#define LCPP_GC_AlwaysCreateNewSurvivor EZ_OFF

/// Will collect after each allocation.
#define LCPP_GC_CollectBeforeEachAllocation EZ_OFF

/// Will never free allocated memory but leave it alive and (hopefully) protected.
#define LCPP_GC_KeepAllocatedMemory EZ_OFF
