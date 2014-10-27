#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/exceptions/runtimeException.h"

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
        m_uiNumInitialPages(0),
        m_fGrowingThreshold(0.66f)
    {
    }

    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    EZ_FORCE_INLINE
    T* GarbageCollector::createStatic(Ptr<const MetaInfo> pMetaInfo)
    {
        LCPP_LogBlock("GarbageCollector::createStatic");

        auto pInstance = EZ_NEW(m_pAllocator, T);
        pInstance->setGarbageCollector(this);
        pInstance->setMetaInfo(pMetaInfo);

        return pInstance;
    }

    template<typename T>
    EZ_FORCE_INLINE
    void GarbageCollector::destroyStatic(T* pObject)
    {
        EZ_DELETE(m_pAllocator.get(), pObject);
    }

    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::create(Ptr<const MetaInfo> pMetaInfo)
    {
        EZ_CHECK_AT_COMPILETIME((std::is_convertible<T, CollectableBase>::value));

        LCPP_LogBlock("GarbageCollector::create");

        EZ_ASSERT(!m_pAllocator.isNull(), "Not initialized.");
        EZ_ASSERT(!isCollecting(), "Cannot create new objects while collecting!");

        T* pInstance = nullptr;

        ezUInt32 uiNumTries(0);

#if EZ_ENABLED(LCPP_GC_CollectBeforeEachAllocation)
        ++uiNumTries;
        collect(1);
#endif

        while(true)
        {
            ++uiNumTries;
            auto result = m_pEdenSpace->allocate(pInstance);
            if (result.succeeded())
                break;

            if (result.isOutOfMemory())
            {
                if (uiNumTries >= 3)
                {
                    LCPP_THROW(exceptions::Runtime("Garbage collector is out of memory!"));
                }

                collect(sizeof(T));
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

        return pInstance;
    }

    inline
    bool GarbageCollector::isAlive(Ptr<CollectableBase> pCollectable) const
    {
        // Since we cannot say with 100% certainty that an object is alive while we are collecting garbage,
        // we are thinking positive and assume that it is alive.
        if (isCollecting())
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
        auto ptr = reinterpret_cast<byte_t*>(pObject.get());
        return ptr >= m_pEdenSpace->getBeginning() && ptr <= m_pEdenSpace->getAllocationPointer();
    }

    EZ_FORCE_INLINE
    bool GarbageCollector::isSurvivorObject(Ptr<CollectableBase> pObject) const
    {
        auto ptr = reinterpret_cast<byte_t*>(pObject.get());
        return ptr >= m_pSurvivorSpace->getBeginning() && ptr <= m_pSurvivorSpace->getAllocationPointer();
    }

    EZ_FORCE_INLINE
    ScanFunction_t GarbageCollector::getScanFunction(CollectableBase* pObject)
    {
        MetaProperty prop;
        if (pObject->m_pMetaInfo->getProperty(MetaProperty::Builtin::ScanFunction, prop).Failed())
            return nullptr;

        auto scanner = prop.getData().as<ScanFunction_t>();
        EZ_ASSERT(scanner, "Invalid function pointer!");
        return scanner;
    }

    template<typename T>
    EZ_FORCE_INLINE
    void GarbageCollector::addRoot(T*& pCollectable)
    {
        EZ_ASSERT(pCollectable != nullptr, "Cannot register nullptr as root!");
        auto ppCollectable = (CollectableBase**)&pCollectable;
        m_roots.PushBack(ppCollectable);
    }

    template<typename T>
    EZ_FORCE_INLINE
    void GarbageCollector::removeRoot(T*& pCollectable)
    {
        auto ppCollectable = (CollectableBase**)&pCollectable;
        m_roots.RemoveSwap(ppCollectable);
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
