#include "lcpp/core/typeSystem/metaInfo.h"

namespace lcpp
{
    EZ_FORCE_INLINE
    void CollectableBase::setGarbageCollector(Ptr<GarbageCollector> pGarbageCollector)
    {
        m_pGarbageCollector = pGarbageCollector;
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
        m_pMetaInfo = pMetaInfo;
    }

    EZ_FORCE_INLINE
    Ptr<const MetaInfo> CollectableBase::getMetaInfo() const
    {
        return m_pMetaInfo;
    }

    //////////////////////////////////////////////////////////////////////////
    
    EZ_FORCE_INLINE
    GarbageCollector::CInfo::CInfo() :
        m_uiInitialMemoryLimit(128 * 1024 * 1024) // 128 MiB
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
    Ptr<T> GarbageCollector::create(Ptr<const MetaInfo> pMetaInfo)
    {
        LCPP_LogBlock("GarbageCollector::create");
        
        EZ_CHECK_AT_COMPILETIME((std::is_convertible<T, CollectableBase>::value));

        LCPP_LogBlock("GarbageCollector::basicCreate");
        
        T* pInstance = nullptr;

        while(true)
        {
            auto result = m_edenSpace.allocate(pInstance);

            if (result.succeeded())
            {
                break;
            }

            if (result.isOutOfMemory())
            {
                // TODO Collect garbage.
                LCPP_NOT_IMPLEMENTED;
            }
            else
            {
                // TODO Figure out what to do here.
                LCPP_NOT_IMPLEMENTED;
            }
        }

        new (pInstance) T();
        pInstance->setGarbageCollector(this);
        pInstance->setMetaInfo(pMetaInfo);

        return pInstance;
    }

}
