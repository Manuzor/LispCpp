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
            cinfo.m_uiMaxPagesPerPool = 16384;//131072; // 1 GiB
            cinfo.m_pParentAllocator = defaultAllocator();

            instance.initialize(cinfo);
            pGC = &instance;
        }

        return pGC;
    }

    GarbageCollector::GarbageCollector() :
        m_pAllocator(nullptr),
        m_uiNumCollectionPreventions(0),
        m_bGrowBeforeNextCollection(false)
    {
    }

    void GarbageCollector::initialize(const CInfo& cinfo)
    {
        m_pAllocator = cinfo.m_pParentAllocator;

        EZ_ASSERT(cinfo.m_fGrowingThreshold >= 0.0f && cinfo.m_fGrowingThreshold <= 1.0f, "Invalid growing threshold.");
        m_fGrowingThreshold = cinfo.m_fGrowingThreshold;

        EZ_ASSERT(cinfo.m_uiNumInitialPages > 0, "Invalid initial number of pages");
        m_uiNumCurrentPages = cinfo.m_uiNumInitialPages;

        EZ_ASSERT(cinfo.m_uiMaxPagesPerPool >= cinfo.m_uiNumInitialPages, "Invalid number of max allocated pages.");
        m_uiMaxNumPages = cinfo.m_uiMaxPagesPerPool;

        for (int i = 0; i < NumMemoryPools; ++i)
            m_pools[i].initialize(m_uiNumCurrentPages);

        m_pEdenSpace = &m_pools[0];
        m_pSurvivorSpace = &m_pools[1];
        m_pSurvivorSpace->protect();

        m_ScanPointer = nullptr;
        m_uiCurrentGeneration = 0;
        m_bGrowBeforeNextCollection = false;

        memset(m_collectionStats, 0, sizeof(m_collectionStats));
    }

    void GarbageCollector::clear()
    {
        EZ_ASSERT(!m_pAllocator.isNull(), "Not initialized.");

        m_ScanPointer = nullptr;
        collect(0); // Should clean everything up.

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

        // Reset stats.
        memset(m_collectionStats, 0, sizeof(m_collectionStats));

#if EZ_ENABLED(LCPP_GC_AlwaysCreateNewSurvivor)
        if(m_bGrowBeforeNextCollection)
        {
            increaseNumCurrentPages();
            m_bGrowBeforeNextCollection = false;
        }
        m_pSurvivorSpace->initialize(m_uiNumCurrentPages);
#else
        bool bDoGrow = m_bGrowBeforeNextCollection && m_uiNumCurrentPages < m_uiMaxNumPages;
        if (bDoGrow)
        {
            m_bGrowBeforeNextCollection = false;

            ezLog::Dev("Growing GC memory");
            ezLog::Dev("  from %u pages (%u KiB)", m_uiNumCurrentPages, m_uiNumCurrentPages * GarbageCollectorPageSize / 1024);

            increaseNumCurrentPages();
            if(m_uiNumCurrentPages == m_uiMaxNumPages)
                bDoGrow = false;

            ezLog::Dev("  to   %u pages (%u KiB)", m_uiNumCurrentPages, m_uiNumCurrentPages * GarbageCollectorPageSize / 1024);
            printStats();
        }

        if(bDoGrow || m_pSurvivorSpace->getEntireMemorySize() < m_pEdenSpace->getEntireMemorySize())
        {
#if EZ_DISABLED(LCPP_GC_KeepAllocatedMemory)
            m_pSurvivorSpace->free();
#endif
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

        EZ_ASSERT(m_pSurvivorSpace->getEntireMemorySize() >= m_pEdenSpace->getEntireMemorySize(), "");
    }

    void GarbageCollector::collect(std::size_t uiNumMinBytesToFree)
    {
        EZ_ASSERT(m_uiNumCollectionPreventions == 0, "Collection is not enabled at this point.");

        bool bExceptionsEnabled = false;
        LCPP_SCOPE_EXIT{ EZ_ASSERT(bExceptionsEnabled, "An exception was thrown during a gc collection cycle."); };

        ezLog::Dev("Garbage collection started, trying to free at least %u bytes.", uiNumMinBytesToFree);

        while(m_uiNumCurrentPages * GarbageCollectorPageSize - m_pSurvivorSpace->getAllocatedMemorySize() < uiNumMinBytesToFree)
        {
            m_bGrowBeforeNextCollection = true;
            increaseNumCurrentPages();

            if(m_uiNumCurrentPages == m_uiMaxNumPages)
                break;
        }

        prepareCollectionCycle();

        m_ScanPointer = m_pSurvivorSpace->getBeginning();
        LCPP_SCOPE_EXIT{ m_ScanPointer = nullptr; };

        // Make sure all roots stay alive
        addRootsToSurvivorSpace();
        scanSurvivorSpace();

        // Make sure the stack pointers stay valid
        addStackPointersToSurvivorSpace();
        scanSurvivorSpace();

        destroyGarbage();

        finalizeCollectionCycle();

        bExceptionsEnabled = true;
    }

    void GarbageCollector::finalizeCollectionCycle()
    {
        // Protect the garbage.
        m_pEdenSpace->protect();

        std::swap(m_pEdenSpace, m_pSurvivorSpace);

        auto fPercentFilled = m_pEdenSpace->getPercentageFilled();
        m_bGrowBeforeNextCollection = fPercentFilled > m_fGrowingThreshold;
        if (m_bGrowBeforeNextCollection)
        {
            ezLog::Dev("GC will grow next cycle. Currently filled by %f%%", fPercentFilled * 100);
            printStats();
        }

        EZ_ASSERT(m_pEdenSpace->getEntireMemorySize() >= m_pSurvivorSpace->getEntireMemorySize(), "");

        ezLog::Dev("Garbage collection stats:");
        for(int i = 0; i < Type::ENUM_COUNT; ++i)
        {
            if(m_collectionStats[i] == 0)
                continue;
            ezLog::Dev("  %s: %u survived", Type((Type::Enum)i).toString(), m_collectionStats[i]);
        }
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

        // Add to statistics
        m_collectionStats[pResult->m_pMetaInfo->getType().getId()]++;

        return pResult;
    }

    void GarbageCollector::increaseNumCurrentPages()
    {
        ezUInt32 uiNewAmountOfPages = m_uiNumCurrentPages * 2;
        if (uiNewAmountOfPages > m_uiMaxNumPages)
        {
            ezLog::Warning("Reached maximum number of memory pages. Cannot increase allocated memory size from now on.");
            uiNewAmountOfPages = m_uiMaxNumPages;
        }

        m_uiNumCurrentPages = uiNewAmountOfPages;
    }

    void GarbageCollector::printStats()
    {
        ezLog::Dev("Generation %u", m_uiCurrentGeneration);
        ezLog::Dev("Growing threshold: %f%%", m_fGrowingThreshold * 100);
        ezLog::Dev("Max Memory Per Pool: %u KiB (%u Pages)", m_uiMaxNumPages * GarbageCollectorPageSize / 1024, m_uiMaxNumPages);
        ezLog::Dev("Survivor: %u KiB / %u KiB (%u Pages) | %.2f%% full",
                   m_pSurvivorSpace->getAllocatedMemorySize() / 1024,
                   m_pSurvivorSpace->getEntireMemorySize() / 1024,
                   m_pSurvivorSpace->getEntireMemorySize() / GarbageCollectorPageSize,
                   m_pSurvivorSpace->getPercentageFilled() * 100);
        ezLog::Dev("Eden:     %u KiB / %u KiB (%u Pages) | %.2f%% full",
                   m_pEdenSpace->getAllocatedMemorySize() / 1024,
                   m_pEdenSpace->getEntireMemorySize() / 1024,
                   m_pEdenSpace->getEntireMemorySize() / GarbageCollectorPageSize,
                   m_pEdenSpace->getPercentageFilled() * 100);
    }

}
