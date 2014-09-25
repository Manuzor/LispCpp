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
        static GarbageCollector gc = []
        {
            GarbageCollector::CInfo cinfo;
            cinfo.m_pParentAllocator = defaultAllocator();

            return GarbageCollector(cinfo);
        }(); // Note that this lambda is immediately called.

        return &gc;
    }

    GarbageCollector::GarbageCollector(const CInfo& cinfo) :
        m_pAllocator(cinfo.m_pParentAllocator),
        m_roots(cinfo.m_pParentAllocator.get())
    {
        EZ_ASSERT(cinfo.m_uiInitialMemoryLimit > 0, "Invalid initial memory limit");

        const std::size_t memorySize = cinfo.m_uiInitialMemoryLimit;
        byte_t* pMemory = nullptr;

        pMemory = EZ_NEW_RAW_BUFFER(m_pAllocator.get(), byte_t, memorySize);
        EZ_ASSERT(pMemory != nullptr, "Out of memory!");
        m_edenSpace = Array<byte_t>(pMemory, memorySize);

        pMemory = EZ_NEW_RAW_BUFFER(m_pAllocator.get(), byte_t, memorySize);
        EZ_ASSERT(pMemory != nullptr, "Out of memory!");
        m_survivorSpace = Array<byte_t>(pMemory, memorySize);
    }

    void GarbageCollector::collect()
    {
        while(true)
        {
            // TODO collect...
            LCPP_NOT_IMPLEMENTED;
        }

        auto newSurvivorMemory = m_edenSpace.getEntireMemory();
        m_edenSpace = m_survivorSpace;
        m_survivorSpace = newSurvivorMemory;

        LCPP_NOT_IMPLEMENTED;
    }

}
