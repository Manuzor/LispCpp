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
        for (auto& pRoot : m_roots)
        {
            addSurvivor(pRoot);
            // Replace the old location of the root in the m_roots array with the new location.
            pRoot = pRoot->m_pForwardPointer;

            scanAndPatch(pRoot);
        }

        // TODO "destroy" garbage.

        auto newSurvivorMemory = m_edenSpace.getEntireMemory();
        m_edenSpace = m_survivorSpace;
        m_survivorSpace = newSurvivorMemory;
    }

    void GarbageCollector::scanAndPatch(CollectableBase* pObject)
    {
        MetaProperty prop;
        if (pObject->m_pMetaInfo->getProperty(MetaProperty::Builtin::ScanFunction, prop).Failed())
        {
            // The object does not have a scan function, which means there are no pointers to patch.
            return;
        }

        auto scanner = prop.getData().as<ScanFunction>();

        PatchablePointerArray pointersToPatch;
        scanner(Ptr<CollectableBase>(pObject), pointersToPatch);

        for (auto ppToPatch : pointersToPatch)
        {
            auto& pObject = *ppToPatch;

            if (!isValidEdenPtr(pObject))
            {
                continue;
            }

            if (!pObject->m_bIsForwarded)
            {
                addSurvivor(pObject.get());
            }

            // Patch the pointer.
            pObject = pObject->m_pForwardPointer;
        }
    }

    void GarbageCollector::addSurvivor(CollectableBase* pSurvivor)
    {
        byte_t* ptr;
        m_survivorSpace.allocate(ptr, pSurvivor->m_uiMemorySize);
        memcpy(ptr, pSurvivor, pSurvivor->m_uiMemorySize);
        pSurvivor->m_bIsForwarded = true;
        pSurvivor->m_pForwardPointer = reinterpret_cast<CollectableBase*>(ptr);
    }

}
