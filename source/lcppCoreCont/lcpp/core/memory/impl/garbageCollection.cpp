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
            cinfo.m_uiNumPages = 1;
            cinfo.m_pParentAllocator = defaultAllocator();

            return GarbageCollector(cinfo);
        }(); // Note that this lambda is immediately called.

        return &gc;
    }

    GarbageCollector::GarbageCollector(const CInfo& cinfo) :
        m_pAllocator(cinfo.m_pParentAllocator),
        m_uiCurrentGeneration(0),
        m_bIsCollecting(false)
    {
        initialize(cinfo);
    }

    void GarbageCollector::initialize(const CInfo& cinfo)
    {
        EZ_ASSERT(cinfo.m_uiNumPages > 0, "Invalid initial memory limit");

        for (auto i = 0; i < NumMemoryPools; ++i)
            m_pools[i] = FixedMemory(cinfo.m_uiNumPages);
    }

    void GarbageCollector::clear()
    {
        m_stackReferences.Clear();
        collect(); // Should clean everything up.

        byte_t* pMemory(nullptr);

        for (auto i = 0; i < NumMemoryPools; ++i)
            m_pools[i].free();

        m_uiCurrentGeneration = 0;
    }

    void GarbageCollector::collect()
    {
        EZ_ASSERT(m_uiCurrentGeneration < std::numeric_limits<decltype(m_uiCurrentGeneration)>::max(),
                  "Congratulations, you collected a LOT of garbage...");

        EZ_ASSERT(!m_bIsCollecting, "We are already collecting!");

        m_bIsCollecting = true;
        LCPP_SCOPE_EXIT{ m_bIsCollecting = false; };

        ++m_uiCurrentGeneration;

        // Make sure all stack references stay alive
        for (auto pStackPtr : m_stackReferences)
        {
            auto pCollectable = pStackPtr->m_ptr.get();
            if (!pCollectable->isForwarded())
            {
                auto result = addSurvivor(pCollectable);
                if (result.isOutOfMemory())
                {
                    LCPP_NOT_IMPLEMENTED;
                }
            }

            pCollectable = pCollectable->m_pForwardPointer;
            pStackPtr->m_ptr = pCollectable;
            scanAndPatch(pCollectable);
        }

        // Destroy the garbage.
        auto usedEden = m_pEdenSpace->getMemory();
        using IndexType = decltype(usedEden.getSize());
        IndexType i(0);
        IndexType endOfUsedEden(usedEden.getSize());
        while(i < endOfUsedEden)
        {
            auto pCollectable = reinterpret_cast<CollectableBase*>(&usedEden[i]);
            i += pCollectable->m_uiMemorySize;

            if (pCollectable->isForwarded())
            {
                EZ_ASSERT(pCollectable->m_uiGeneration == m_uiCurrentGeneration - 1,
                          "If the object was valid and has been added as a survivor, "
                          "it must have happened within this collection cycle.");
                continue;
            }

            EZ_ASSERT(pCollectable->isAlive(),
                      "Object should be considered alive at this point so we can destroy it. "
                      "If it is not alive, it must have been destroyed already.");

            pCollectable->m_state = GarbageState::Destroying;

            MetaProperty destructorProperty;
            if (pCollectable->m_pMetaInfo->getProperty(MetaProperty::Builtin::DestructorFunction, destructorProperty).Succeeded())
            {
                auto destructorFunction = destructorProperty.getData().as<DestructorFunction_t>();
                (*destructorFunction)(pCollectable);
            }

            pCollectable->m_state = GarbageState::Garbage;
        }

        std::swap(m_pEdenSpace, m_pSurvivorSpace);
    }

    void GarbageCollector::scanAndPatch(CollectableBase* pObject)
    {
        MetaProperty prop;
        if (pObject->m_pMetaInfo->getProperty(MetaProperty::Builtin::ScanFunction, prop).Failed())
        {
            // The object does not have a scan function, which means there are no pointers to patch.
            return;
        }

        auto scanner = prop.getData().as<ScanFunction_t>();

        PatchablePointerArray_t pointersToPatch;
        scanner(pObject, pointersToPatch);

        for (auto ppToPatch : pointersToPatch)
        {
            auto& pToPatch = *ppToPatch;

            if (!isEdenObject(pToPatch))
                continue;

            if (!pToPatch->isForwarded())
            {
                auto result = addSurvivor(pToPatch);
                if (result.isOutOfMemory())
                {
                    LCPP_NOT_IMPLEMENTED;
                }
            }

            // Patch the pointer.
            pToPatch = pToPatch->m_pForwardPointer;
        }
    }

    AllocatorResult GarbageCollector::addSurvivor(CollectableBase* pSurvivor)
    {
        byte_t* ptr;
        auto result = m_pSurvivorSpace->allocate(ptr, pSurvivor->m_uiMemorySize);
        if (!result.succeeded())
            return result;

        memcpy(ptr, pSurvivor, pSurvivor->m_uiMemorySize);
        pSurvivor->m_state = GarbageState::Forwarded;
        pSurvivor->m_pForwardPointer = reinterpret_cast<CollectableBase*>(ptr);
        pSurvivor->m_pForwardPointer->m_uiGeneration = m_uiCurrentGeneration;

        return result;
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
