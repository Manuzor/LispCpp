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
        static GarbageCollector gc(defaultAllocator());
        return &gc;
    }

    GarbageCollector::GarbageCollector(Ptr<ezAllocatorBase> pParentAllocator) :
        m_pAllocator(pParentAllocator)
    {
        const std::size_t memorySize = 128 * 1024 * 1024;
        auto pMemory = EZ_NEW_RAW_BUFFER(m_pAllocator.get(), byte_t, memorySize);
        m_memory = Array<byte_t>(pMemory, memorySize);
    }

    void GarbageCollector::collect()
    {
        LCPP_NOT_IMPLEMENTED;
    }

}
