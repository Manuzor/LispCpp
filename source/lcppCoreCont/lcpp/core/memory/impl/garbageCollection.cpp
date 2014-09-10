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

    GarbageCollector::GarbageCollector(Ptr<ezAllocatorBase> pParentAllocator) :
        m_pAllocator(pParentAllocator)
    {
        {
            const std::size_t memorySize = 1024;
            auto pMemory = (byte_t*)m_pAllocator->Allocate(memorySize, EZ_ALIGNMENT_OF(byte_t));
            m_staticAllocations = Array<byte_t>(pMemory, memorySize);
        }

        {
            const std::size_t memorySize = 128 * 1024 * 1024;
            auto pMemory = (byte_t*)m_pAllocator->Allocate(memorySize, EZ_ALIGNMENT_OF(byte_t));
            m_dynamicAllocations = Array<byte_t>(pMemory, memorySize);
        }
    }

    void GarbageCollector::collect()
    {
        LCPP_NOT_IMPLEMENTED;
    }

}
