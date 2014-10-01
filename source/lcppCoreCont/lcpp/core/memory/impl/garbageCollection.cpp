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
            cinfo.m_uiInitialMemoryLimit = 128 * 1024 * 1024; // 128 MiB
            //cinfo.m_uiInitialMemoryLimit = 1024;
            cinfo.m_pParentAllocator = defaultAllocator();

            return GarbageCollector(cinfo);
        }(); // Note that this lambda is immediately called.

        return &gc;
    }

    GarbageCollector::GarbageCollector(const CInfo& cinfo) :
        m_pAllocator(cinfo.m_pParentAllocator),
        m_roots(cinfo.m_pParentAllocator.get()),
        m_uiCurrentGeneration(0),
        m_bIsCollecting(false)
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
        EZ_ASSERT(m_uiCurrentGeneration < std::numeric_limits<decltype(m_uiCurrentGeneration)>::max(),
                  "Congratulations, you collected a LOT of garbage...");

        EZ_ASSERT(!m_bIsCollecting, "We are already collecting!");

        m_bIsCollecting = true;
        LCPP_SCOPE_EXIT{ m_bIsCollecting = false; };

        ++m_uiCurrentGeneration;

        for (auto& pRoot : m_roots)
        {
            addSurvivor(pRoot);
            // Replace the old location of the root in the m_roots array with the new location.
            pRoot = pRoot->m_pForwardPointer;

            scanAndPatch(pRoot);
        }

        // Make sure all stack reference stay alive
        for (auto pStackPtr : m_stackReferences)
        {
            auto pCollectable = pStackPtr->m_ptr.get();
            if (!pCollectable->m_bIsForwarded)
                addSurvivor(pCollectable);

            pCollectable = pCollectable->m_pForwardPointer;
            scanAndPatch(pCollectable);

            pStackPtr->m_ptr = pCollectable;
        }

        // Call the garbage's destroy functions.
        auto usedEden = m_edenSpace.getMemory();
        using IndexType = decltype(usedEden.getSize());
        IndexType i(0);
        IndexType endOfUsedEden(usedEden.getSize());
        while(i < endOfUsedEden)
        {
            auto mem = &usedEden[i];
            auto pCollectable = reinterpret_cast<CollectableBase*>(mem);
            i += pCollectable->m_uiMemorySize;

            if (pCollectable->m_uiGeneration == m_uiCurrentGeneration)
                continue;

            MetaProperty destructorProperty;
            if (pCollectable->m_pMetaInfo->getProperty(MetaProperty::Builtin::DestructorFunction, destructorProperty).Succeeded())
            {
                auto destructorFunction = destructorProperty.getData().as<DestructorFunction_t>();
                (*destructorFunction)(pCollectable);
            }
        }

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
        scanner(pObject, pointersToPatch);

        for (auto ppToPatch : pointersToPatch)
        {
            auto& pToPatch = *ppToPatch;

            if (!isValidEdenPtr(pToPatch))
                continue;

            if (!pToPatch->m_bIsForwarded)
            {
                addSurvivor(pToPatch);
            }

            // Patch the pointer.
            pToPatch = pToPatch->m_pForwardPointer;
        }
    }

    void GarbageCollector::addSurvivor(CollectableBase* pSurvivor)
    {
        byte_t* ptr;
        auto result = m_survivorSpace.allocate(ptr, pSurvivor->m_uiMemorySize);
        EZ_ASSERT(result.succeeded(), "Out of memory...");
        memcpy(ptr, pSurvivor, pSurvivor->m_uiMemorySize);
        pSurvivor->m_bIsForwarded = true;
        pSurvivor->m_pForwardPointer = reinterpret_cast<CollectableBase*>(ptr);
        pSurvivor->m_pForwardPointer->m_uiGeneration = m_uiCurrentGeneration;
    }

    bool GarbageCollector::isOnStack(Ptr<CollectableBase> pCollectable) const
    {
        for (auto pStackPtr : m_stackReferences)
        {
            if (pStackPtr->m_ptr == pCollectable)
                return true;
        }

        return false;
    }
}
