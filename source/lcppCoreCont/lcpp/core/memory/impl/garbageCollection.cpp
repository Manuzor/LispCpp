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
            cinfo.m_uiNumInitialPages = 1;
            cinfo.m_pParentAllocator = defaultAllocator();

            return GarbageCollector(cinfo);
        }(); // Note that this lambda is immediately called.

        return &gc;
    }

    GarbageCollector::GarbageCollector(const CInfo& cinfo) :
        m_pAllocator(cinfo.m_pParentAllocator),
        m_uiNumCurrentPages(cinfo.m_uiNumInitialPages),
        m_uiCurrentGeneration(0),
        m_ScanPointer(nullptr)
    {
        initialize(cinfo);
    }

    void GarbageCollector::initialize(const CInfo& cinfo)
    {
        EZ_ASSERT(cinfo.m_uiNumInitialPages > 0, "Invalid initial number of pages");
        m_uiNumCurrentPages = cinfo.m_uiNumInitialPages;
        m_pools.Clear();
        m_pools.SetCountUninitialized(NumMemoryPools);

        for (auto& pool : m_pools)
            pool = FixedMemory(m_uiNumCurrentPages);

        m_pEdenSpace = &m_pools[0];
        m_pSurvivorSpace = &m_pools[1];
        m_pSurvivorSpace->protect();

        m_ScanPointer = nullptr;
    }

    void GarbageCollector::clear()
    {
        m_ScanPointer = nullptr;
        collect(); // Should clean everything up.

        for (auto& pool : m_pools)
            pool.free();

        m_uiCurrentGeneration = 0;
    }

    void GarbageCollector::collect()
    {
        EZ_ASSERT(m_uiCurrentGeneration < std::numeric_limits<decltype(m_uiCurrentGeneration)>::max(),
                  "Congratulations, you collected a LOT of garbage...");

        EZ_ASSERT(!isCollecting(), "We are already collecting!");

        ++m_uiCurrentGeneration;

#if EZ_ENABLED(LCPP_GC_AlwaysCreateNewSurvivor)
        // Note: Survivor is already protected at this point.
        *m_pSurvivorSpace = FixedMemory(m_uiNumCurrentPages);
#endif

        m_ScanPointer = m_pSurvivorSpace->getBeginning();

        // Make sure all roots stay alive
        for (auto ppRoot : m_roots)
        {
            auto& pRoot = *ppRoot;
            pRoot = addSurvivor(pRoot);
            if (pRoot == nullptr)
            {
                LCPP_NOT_IMPLEMENTED;
            }
        }

        scanSurvivors();

        // Scan and patch all stack pointers.
        for (ezUInt32 i = 0; i < StackPtrBase::s_uiNextIndex; ++i)
        {
            auto& pCollectable = StackPtrBase::s_ptrTable[i];
            pCollectable = addSurvivor(pCollectable);
            if (pCollectable == nullptr)
            {
                LCPP_NOT_IMPLEMENTED;
            }
        }

        scanSurvivors();

        destroyGarbage();

        std::swap(m_pEdenSpace, m_pSurvivorSpace);

        m_pSurvivorSpace->protect();
        m_ScanPointer = nullptr;
    }

    void GarbageCollector::destroyGarbage()
    {
        auto pMemory = m_pEdenSpace->getBeginning();
        auto pMemoryEnd = m_pEdenSpace->getAllocationPointer();
        while(pMemory < pMemoryEnd)
        {
            auto pCollectable = reinterpret_cast<CollectableBase*>(pMemory);
            pMemory += pCollectable->m_uiMemorySize;

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

            MetaProperty destructorProperty;
            if (pCollectable->m_pMetaInfo->getProperty(MetaProperty::Builtin::DestructorFunction, destructorProperty).Succeeded())
            {
                pCollectable->m_state = GarbageState::Destroying;
                auto destructorFunction = destructorProperty.getData().as<DestructorFunction_t>();
                (*destructorFunction)(pCollectable);
            }

            pCollectable->m_state = GarbageState::Garbage;
        }
    }

    void GarbageCollector::scanAndPatch(CollectableBase* pObject)
    {
        auto scanner = getScanFunction(pObject);

        if (scanner == nullptr)
        {
            // The object has no scan function, which means there's nothing to scan or patch
            return;
        }

        // The objects are responsible for patching themselves.
        scanner(pObject, this);
    }

    ezUInt64 GarbageCollector::scanSurvivors()
    {
        ezUInt64 uiScanCount(0);
        while(m_ScanPointer < m_pSurvivorSpace->getAllocationPointer())
        {
            auto pToScan = reinterpret_cast<CollectableBase*>(m_ScanPointer);
            scanAndPatch(pToScan);
            ++uiScanCount;
        }
        return uiScanCount;
    }

    CollectableBase* GarbageCollector::addSurvivor(CollectableBase* pSurvivor)
    {
        EZ_ASSERT(isCollecting(), "Can only add survivors while collecting!");

        if(pSurvivor->isForwarded())
            return pSurvivor->m_pForwardPointer;

        byte_t* ptr;
        auto result = m_pSurvivorSpace->allocate(ptr, pSurvivor->m_uiMemorySize);
        if (!result.succeeded())
            return nullptr;

        memcpy(ptr, pSurvivor, pSurvivor->m_uiMemorySize);
        auto pResult = reinterpret_cast<CollectableBase*>(ptr);
        pResult->m_uiGeneration = m_uiCurrentGeneration;

        pSurvivor->m_state = GarbageState::Forwarded;
        pSurvivor->m_pForwardPointer = pResult;

        return pResult;
    }

}
