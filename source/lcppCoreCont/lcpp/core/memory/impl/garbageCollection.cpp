#include "stdafx.h"
#include "lcpp/core/memory/garbageCollection.h"

namespace lcpp
{
    GarbageCollector* getGarbageCollector()
    {
        static GarbageCollector allocator(defaultAllocator());
        return &allocator;
    }

    GarbageCollector::GarbageCollector(ezAllocatorBase* pParentAllocator) :
        m_pParent(pParentAllocator),
        m_stats(),
        m_data(pParentAllocator),
        m_pEdenSpace(nullptr),
        m_pSurvivorSpace(nullptr),
        m_uiAllocationIndex(0)
    {
        ezMemoryTracker::RegisterAllocator("lcpp/CollectingAllocator", (ezMemoryTrackingFlags::Enum)0);

        m_data.SetCountUninitialized(128 * 1024 * 1024); // 128 MiB

        m_pEdenSpace = &m_data.m_left;
        m_pSurvivorSpace = &m_data.m_right;
        
        EZ_ASSERT(m_pEdenSpace != m_pSurvivorSpace, "");
    }

    void GarbageCollector::collect()
    {
        LCPP_NOT_IMPLEMENTED;
    }

    void* GarbageCollector::Allocate(size_t uiSize, size_t uiAlign)
    {
        m_data.EnsureRangeIsValid(m_uiAllocationIndex, ezUInt32(uiSize));

        auto pMem = &(*m_pEdenSpace)[m_uiAllocationIndex];

        m_uiAllocationIndex += ezUInt32(uiSize);

        // Update stats.
        ++m_stats.m_uiNumAllocations;
        m_stats.m_uiAllocationSize += uiSize;

        return static_cast<void*>(pMem);
    }

    void GarbageCollector::Deallocate(void* ptr)
    {
        ++m_stats.m_uiNumDeallocations;

        // TODO Really don't do anything here?
    }

    size_t GarbageCollector::AllocatedSize(const void* ptr)
    {
        // TODO Implement this?
        return 0;
    }

    ezAllocatorBase::Stats GarbageCollector::GetStats() const
    {
        return m_stats;
    }

}
