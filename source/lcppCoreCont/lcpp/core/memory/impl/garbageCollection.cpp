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
        static GarbageCollector* pGC = nullptr;

        if (pGC == nullptr)
        {
            static GarbageCollector instance;

            GarbageCollector::CInfo cinfo;
            cinfo.m_uiNumInitialPages = 1;
            cinfo.m_pParentAllocator = defaultAllocator();

            instance.initialize(cinfo);
            pGC = &instance;
        }

        return pGC;
    }

    GarbageCollector::GarbageCollector() :
        m_pAllocator(nullptr),
        m_uiNumCollectionPreventions(0)
    {
    }

    void GarbageCollector::initialize(const CInfo& cinfo)
    {
        m_pAllocator = cinfo.m_pParentAllocator;

        EZ_ASSERT(cinfo.m_fGrowingThreshold >= 0.0f && cinfo.m_fGrowingThreshold <= 1.0f, "Invalid growing threshold.");
        m_fGrowingThreshold = cinfo.m_fGrowingThreshold;

        EZ_ASSERT(cinfo.m_uiNumInitialPages > 0, "Invalid initial number of pages");
        m_uiNumCurrentPages = cinfo.m_uiNumInitialPages;

        for (int i = 0; i < NumMemoryPools; ++i)
            m_pools[i].initialize(m_uiNumCurrentPages);

        m_pEdenSpace = &m_pools[0];
        m_pSurvivorSpace = &m_pools[1];
        m_pSurvivorSpace->protect();

        m_ScanPointer = nullptr;
        m_uiCurrentGeneration = 0;
    }

    void GarbageCollector::clear()
    {
        EZ_ASSERT(!m_pAllocator.isNull(), "Not initialized.");

        m_ScanPointer = nullptr;
        collect(); // Should clean everything up.

        m_pEdenSpace = nullptr;
        m_pSurvivorSpace = nullptr;

        for (int i = 0; i < NumMemoryPools; ++i)
        {
            m_pools[i].free();
        }

        m_uiCurrentGeneration = std::numeric_limits<decltype(m_uiCurrentGeneration)>::max();
    }

    void GarbageCollector::prepareCollectionCycle()
    {
        EZ_ASSERT(!m_pAllocator.isNull(), "Not initialized.");

        EZ_ASSERT(m_uiCurrentGeneration < std::numeric_limits<decltype(m_uiCurrentGeneration)>::max(),
                  "Congratulations, you collected a LOT of garbage...");

        EZ_ASSERT(!isCollecting(), "We are already collecting!");

        ++m_uiCurrentGeneration;

#if EZ_ENABLED(LCPP_GC_AlwaysCreateNewSurvivor)
        if(m_bGrowBeforeNextCollection)
        {
            m_uiNumCurrentPages *= 2;
            m_bGrowBeforeNextCollection = false;
        }
        m_pSurvivorSpace->initialize(m_uiNumCurrentPages);
#else
        if (m_bGrowBeforeNextCollection)
        {
            m_bGrowBeforeNextCollection = false;
            m_uiNumCurrentPages *= 2;
            m_pSurvivorSpace->free();
            // Note: Survivor is already protected at this point.
            m_pSurvivorSpace->initialize(m_uiNumCurrentPages);
        }
        else
        {
            // Prepare the survivor space for new allocations, discarding all garbage it contains.
            m_pSurvivorSpace->reset();
        }
#endif
        //printf("Eden range:     0x%016llX - 0x%016llX\n", reinterpret_cast<ezUInt64>(m_pEdenSpace->getBeginning()), reinterpret_cast<ezUInt64>(m_pEdenSpace->getEnd()));
        //printf("Survivor range: 0x%016llX - 0x%016llX\n", reinterpret_cast<ezUInt64>(m_pSurvivorSpace->getBeginning()), reinterpret_cast<ezUInt64>(m_pSurvivorSpace->getEnd()));

        m_ScanPointer = m_pSurvivorSpace->getBeginning();
    }

    void GarbageCollector::collect()
    {
        EZ_ASSERT(m_uiNumCollectionPreventions == 0, "Collection is not enabled at this point.");

        prepareCollectionCycle();

        // Make sure all roots stay alive
        addRootsToSurvivorSpace();
        scanSurvivorSpace();

        // Make sure the stack pointers stay valid
        addStackPointersToSurvivorSpace();
        scanSurvivorSpace();

        destroyGarbage();

        for (ezUInt32 i = 0; i < StackPtrBase::s_uiNextIndex; ++i)
        {
            auto pObject = StackPtrBase::s_ptrTable[i];

        }

        finalizeCollectionCycle();
    }

    void GarbageCollector::finalizeCollectionCycle()
    {
        // Protect the garbage.
        m_pEdenSpace->protect();

        std::swap(m_pEdenSpace, m_pSurvivorSpace);

        // Reset the scan pointer.
        m_ScanPointer = nullptr;

        m_bGrowBeforeNextCollection = m_pEdenSpace->getPercentageFilled() > m_fGrowingThreshold;
    }

    void GarbageCollector::addRootsToSurvivorSpace()
    {
        for (auto ppRoot : m_roots)
        {
            auto& pRoot = *ppRoot;
            pRoot = addSurvivor(pRoot);
            if (pRoot == nullptr)
            {
                LCPP_NOT_IMPLEMENTED;
            }
        }
    }

    void GarbageCollector::addStackPointersToSurvivorSpace()
    {
        for (ezUInt32 i = 0; i < StackPtrBase::s_uiNextIndex; ++i)
        {
            auto& pCollectable = StackPtrBase::s_ptrTable[i];
            pCollectable = addSurvivor(pCollectable);
            if (pCollectable == nullptr)
            {
                LCPP_NOT_IMPLEMENTED;
            }
        }
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

    ezUInt64 GarbageCollector::scanSurvivorSpace()
    {
        ezUInt64 uiScanCount(0);
        while(m_ScanPointer < m_pSurvivorSpace->getAllocationPointer())
        {
            auto pToScan = reinterpret_cast<CollectableBase*>(m_ScanPointer);

            auto scanner = getScanFunction(pToScan);
            if (scanner)
            {
                EZ_ASSERT(pToScan->isAlive(), "");
                // The objects are responsible for patching themselves.
                scanner(pToScan, this);
            }
            ++uiScanCount;
            m_ScanPointer += pToScan->m_uiMemorySize;
        }
        return uiScanCount;
    }

    CollectableBase* GarbageCollector::addSurvivor(CollectableBase* pSurvivor)
    {
        EZ_ASSERT(isCollecting(), "Can only add survivors while collecting!");
        EZ_ASSERT(pSurvivor != nullptr, "nullptr not allowed as survivor!");

        if(!isEdenObject(pSurvivor))
            return pSurvivor; // It's not managed by this collector.

        if(pSurvivor->isForwarded())
            return pSurvivor->m_pForwardPointer;

        byte_t* ptr;
        auto result = m_pSurvivorSpace->allocate(ptr, pSurvivor->m_uiMemorySize);
        if (!result.succeeded())
        {
            return nullptr;
        }

        memcpy(ptr, pSurvivor, pSurvivor->m_uiMemorySize);
        auto pResult = reinterpret_cast<CollectableBase*>(ptr);
        pResult->m_uiGeneration = m_uiCurrentGeneration;

        pSurvivor->m_state = GarbageState::Forwarded;
        pSurvivor->m_pForwardPointer = pResult;

        return pResult;
    }

}
