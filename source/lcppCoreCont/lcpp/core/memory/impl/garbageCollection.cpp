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
        m_pAllocator(pParentAllocator),
        m_uiAllocationIndex(0),
        m_uiStaticAllocationIndex(0)
    {
        m_id = ezMemoryTracker::RegisterAllocator("lcpp/GarbageCollector", (ezMemoryTrackingFlags::Enum)0);

        //m_data.SetCountUninitialized(128 * 1024 * 1024);

        {
            const std::size_t memorySize = 1024;
            auto pMemory = (byte_t*)m_pAllocator->Allocate(memorySize, EZ_ALIGNMENT_OF(byte_t));
            m_staticMemory.assign(pMemory, memorySize);
        }

        {
            const std::size_t memorySize = 128 * 1024 * 1024;
            auto pMemory = (byte_t*)m_pAllocator->Allocate(memorySize, EZ_ALIGNMENT_OF(byte_t));
            m_memory.assign(static_cast<byte_t*>(pMemory), memorySize);
        }
    }

    GarbageCollector::~GarbageCollector()
    {
        ezMemoryTracker::DeregisterAllocator(m_id);
        m_pAllocator = nullptr;
    }

    void GarbageCollector::collect()
    {
        LCPP_NOT_IMPLEMENTED;
    }

    void* GarbageCollector::allocate(size_t uiSize, size_t uiAlign)
    {
        auto uiNewSize = m_eden.getSize() + uiSize;

        if (uiNewSize > m_memory.getSize())
        {
            // Resize memory.
            const std::size_t newMemorySize = m_memory.getSize() * 2;
            auto pNewMemory = (byte_t*)m_pAllocator->Allocate(newMemorySize, EZ_ALIGNMENT_OF(byte_t));

            memcpy(pNewMemory, m_memory.getData(), m_memory.getSize());

            m_pAllocator->Deallocate(m_memory.getData());
            m_memory.assign(pNewMemory, newMemorySize);
            m_eden = m_memory(0, m_eden.getSize());
        }

        EZ_ASSERT(m_uiAllocationIndex == m_eden.getSize(), "Invalid allocation index or eden space.");

        m_eden = m_memory(0, uiNewSize);

        auto pMemory = &m_eden[m_uiAllocationIndex];

        m_uiAllocationIndex += uiSize;

        // Update stats.
        ++m_stats.m_uiNumAllocations;
        m_stats.m_uiAllocationSize += uiSize;

        EZ_ASSERT(m_stats.m_uiAllocationSize == m_eden.getSize(), "Data out of sync!");

        return static_cast<void*>(pMemory);
    }

    void GarbageCollector::deallocate(void* ptr)
    {
        ++m_stats.m_uiNumDeallocations;

        // TODO Really don't do anything here?
    }

    void* GarbageCollector::allocateStatic(size_t uiSize, size_t uiAlign)
    {
        if (m_uiStaticAllocationIndex + uiSize >= m_staticMemory.getSize())
        {
            // Resize static memory.
            const std::size_t newMemorySize = m_staticMemory.getSize() * 2;
            auto pNewMemory = (byte_t*)m_pAllocator->Allocate(newMemorySize, EZ_ALIGNMENT_OF(byte_t));

            memcpy(pNewMemory, m_staticMemory.getData(), m_staticMemory.getSize());

            m_pAllocator->Deallocate(m_staticMemory.getData());
            m_staticMemory.assign(pNewMemory, newMemorySize);
        }

        auto pMemory = &m_staticMemory[m_uiStaticAllocationIndex];

        m_uiStaticAllocationIndex += uiSize;

        // Update stats.
        ++m_statsStatics.m_uiNumAllocations;
        m_statsStatics.m_uiAllocationSize += uiSize;

        return static_cast<void*>(pMemory);
    }

    void GarbageCollector::deallocateStatic(void* ptr)
    {
        ++m_statsStatics.m_uiNumDeallocations;

        // TODO Really don't do anything here?
    }

}
