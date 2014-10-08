#include "lcpp/core/typeSystem/metaInfo.h"

namespace lcpp
{
    EZ_FORCE_INLINE
    CollectableBase::CollectableBase() :
        m_state(GarbageState::Initial),
        m_uiMemorySize(0),
        m_uiGeneration(0)
    {
    }

    EZ_FORCE_INLINE
    void CollectableBase::setGarbageCollector(Ptr<GarbageCollector> pGarbageCollector)
    {
        m_pGarbageCollector = pGarbageCollector.get();
    }

    EZ_FORCE_INLINE
    Ptr<GarbageCollector> CollectableBase::getGarbageCollector()
    {
        return m_pGarbageCollector;
    }

    EZ_FORCE_INLINE
    Ptr<const GarbageCollector> CollectableBase::getGarbageCollector() const
    {
        return m_pGarbageCollector;
    }

    EZ_FORCE_INLINE
    void CollectableBase::setMetaInfo(Ptr<const MetaInfo> pMetaInfo)
    {
        m_pMetaInfo = pMetaInfo.get();
    }

    EZ_FORCE_INLINE
    Ptr<const MetaInfo> CollectableBase::getMetaInfo() const
    {
        return m_pMetaInfo;
    }

    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    GarbageCollector::CInfo::CInfo() :
        m_uiNumPages(0)
    {
    }

    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::createStatic(Ptr<const MetaInfo> pMetaInfo)
    {
        LCPP_LogBlock("GarbageCollector::createStatic");

        auto pInstance = EZ_NEW(m_pAllocator, T);
        pInstance->setGarbageCollector(this);
        pInstance->setMetaInfo(pMetaInfo);

        return pInstance;
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T> GarbageCollector::create(Ptr<const MetaInfo> pMetaInfo)
    {
        EZ_CHECK_AT_COMPILETIME((std::is_convertible<T, CollectableBase>::value));

        LCPP_LogBlock("GarbageCollector::create");

        EZ_ASSERT(!m_bIsCollecting, "Cannot create new objects while collecting!");

        T* pInstance = nullptr;

        ezUInt32 uiNumTries(0);
        while(true)
        {
            ++uiNumTries;
            auto result = m_pEdenSpace->allocate(pInstance);

            if (result.succeeded())
            {
                break;
            }

            if (result.isOutOfMemory())
            {
                if (uiNumTries >= 2)
                {
                    // TODO Resize the internal array?
                    EZ_REPORT_FAILURE("Out of memory!");
                    return nullptr;
                }

                collect();
            }
            else
            {
                // TODO Figure out what to do here.
                LCPP_NOT_IMPLEMENTED;
            }
        }

        new (pInstance) T();
        pInstance->m_uiMemorySize = sizeof(T);
        pInstance->m_pGarbageCollector = this;
        pInstance->m_pMetaInfo = pMetaInfo.get();
        pInstance->m_uiGeneration = m_uiCurrentGeneration;
        pInstance->m_state = GarbageState::Alive;

#if EZ_ENABLED(LCPP_GC_CollectAfterAllocation)
        StackPtr<T> pSafeInstance(pInstance);
        collect();
        pSafeInstance = nullptr;
#endif

        return pInstance;
    }

    inline
    bool GarbageCollector::isAlive(Ptr<CollectableBase> pCollectable) const
    {
        // Since we cannot say with 100% certainty that an object is alive while we are collecting garbage,
        // we are thinking positive and assume that it is alive.
        if (m_bIsCollecting)
            return true;

        if (isEdenObject(pCollectable))
        {
            EZ_ASSERT(pCollectable->m_uiGeneration <= m_uiCurrentGeneration, "Invalid generation for object.");

            if(pCollectable->isAlive())
            {
                EZ_ASSERT(pCollectable->m_uiGeneration == m_uiCurrentGeneration, "Invalid generation for object.");
                return true;
            }
        }
        else if(isSurvivorObject(pCollectable))
        {
            return false;
        }

        // It is not our pointer, so we have to assume it is alive.
        return true;
    }

    EZ_FORCE_INLINE
    bool GarbageCollector::isEdenObject(Ptr<CollectableBase> pObject) const
    {
        auto ptr = (byte_t*)pObject.get();
        auto memory = m_pEdenSpace->getMemory();
        return ptr >= memory.getData() && ptr < (memory.getData() + memory.getSize());
    }

    EZ_FORCE_INLINE
    bool GarbageCollector::isSurvivorObject(Ptr<CollectableBase> pObject) const
    {
        auto ptr = (byte_t*)pObject.get();
        auto memory = m_pSurvivorSpace->getEntireMemory();
        return ptr >= memory.getData() && ptr < (memory.getData() + memory.getSize());
    }

    EZ_FORCE_INLINE
    void GarbageCollector::addStackPtr(const StackPtrBase* stackPtr) const
    {
        EZ_ASSERT(!m_bIsCollecting, "You are not allowed to add a stack pointer while we're collecting!");

        // Prevent adding StackPtr of items that cannot be collected, such as nil, void_, etc.
        if(!isEdenObject(stackPtr->m_ptr)) return;

        m_stackReferences.PushBack(stackPtr);
    }

    EZ_FORCE_INLINE
    void GarbageCollector::removeStackPtr(const StackPtrBase* stackPtr) const
    {
        EZ_ASSERT(!m_bIsCollecting, "You are not allowed to remove a stack pointer while we're collecting!");

        // Prevent removal of StackPtr of items that cannot be collected, such as nil, void_, etc.
        if(!isEdenObject(stackPtr->m_ptr)) return;

        auto uiIndex = m_stackReferences.LastIndexOf(stackPtr);
        EZ_ASSERT(uiIndex != ezInvalidIndex, "");
        m_stackReferences.RemoveAtSwap(uiIndex);
    }

    EZ_FORCE_INLINE
    void detail::assertObjectIsAlive(Ptr<CollectableBase> pCollectable)
    {
        assertObjectIsAlive(pCollectable.get());
    }

    EZ_FORCE_INLINE
    void detail::assertObjectIsAlive(CollectableBase* pCollectable)
    {
        EZ_ASSERT(pCollectable, "Invalid pointer.");

        auto pGc = pCollectable->getGarbageCollector().get();
        EZ_ASSERT(pGc, "The object was not created with a (properly implemented) garbage collector.");

        EZ_ASSERT(pGc->isAlive(pCollectable), "The object is not alive at this point!");
    }
}
