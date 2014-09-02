#include "stdafx.h"

#ifndef VerboseDebugMessage
// Enable this to allow verbose debug messages
#define VerboseDebugMessage Debug
#endif

#include "lcpp/core/memory/garbageCollection.h"

namespace lcpp
{
    GarbageCollector* getGarbageCollector()
    {
        static GarbageCollector allocator(defaultAllocator());
        return &allocator;
    }

    GarbageCollector::GarbageCollector(ezAllocatorBase* pParentAllocator) :
        m_stats(),
        m_data(pParentAllocator),
        m_pEdenSpace(nullptr),
        m_pSurvivorSpace(nullptr),
        m_uiAllocationIndex(0)
    {
        m_id = ezMemoryTracker::RegisterAllocator("lcpp/GarbageCollector", (ezMemoryTrackingFlags::Enum)0);

        //m_data.SetCountUninitialized(128 * 1024 * 1024);

        m_pEdenSpace = &m_data.m_left;
        m_pSurvivorSpace = &m_data.m_right;
        
        EZ_ASSERT(m_pEdenSpace != m_pSurvivorSpace, "");
    }

    GarbageCollector::~GarbageCollector()
    {
        ezMemoryTracker::DeregisterAllocator(m_id);
    }

    void GarbageCollector::collect()
    {
        LCPP_NOT_IMPLEMENTED;
    }

    void* GarbageCollector::Allocate(size_t uiSize, size_t uiAlign)
    {
        auto dataAction = m_data.EnsureRangeIsValid(m_uiAllocationIndex, ezUInt32(uiSize));
        if (dataAction.wasResized())
        {
            // Do anything here?
        }

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
