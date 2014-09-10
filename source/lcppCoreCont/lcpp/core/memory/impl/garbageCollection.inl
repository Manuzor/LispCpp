
namespace lcpp
{
    EZ_FORCE_INLINE
    RefIndex CollectableBase::getRefIndex() const
    {
        return m_refIndex;
    }

    EZ_FORCE_INLINE
    void CollectableBase::setRefIndex(RefIndex refIndex)
    {
        m_refIndex = refIndex;
    }
    
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

    //////////////////////////////////////////////////////////////////////////
    
    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::createStatic()
    {
        EZ_CHECK_AT_COMPILETIME((std::is_convertible<T, CollectableBase>::value));

        LCPP_LogBlock("GarbageCollector::createStatic");

        RefIndex refIndex;
        refIndex.m_bIsStatic = true;
        refIndex.m_uiHash = 0;
        refIndex.m_uiIndex = m_staticAllocations.getAllocationPointer();
        
        T* pInstance = nullptr;

        while(true)
        {
            auto result = m_staticAllocations.allocate(pInstance);

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

        EZ_ASSERT(refIndex.m_uiIndex < m_staticAllocations.getAllocationPointer(), "");
        
        new (pInstance) T();
        pInstance->setGarbageCollector(this);
        pInstance->setRefIndex(refIndex);

        return pInstance;
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::create()
    {
        LCPP_LogBlock("GarbageCollector::create");

        RefIndex refIndex;
        refIndex.m_bIsStatic = false;
        refIndex.m_uiHash = 0;
        refIndex.m_uiIndex = m_dynamicAllocations.getAllocationPointer();
        
        T* pInstance = nullptr;

        while(true)
        {
            auto result = m_dynamicAllocations.allocate(pInstance);

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

        EZ_ASSERT(refIndex.m_uiIndex < m_dynamicAllocations.getAllocationPointer(), "");
        
        new (pInstance) T();
        pInstance->setGarbageCollector(this);
        pInstance->setRefIndex(refIndex);

        return pInstance;
    }

    template<typename T>
    EZ_FORCE_INLINE
    T* GarbageCollector::getPointer(RefIndex refIndex) const
    {
        EZ_CHECK_AT_COMPILETIME((std::is_convertible<T, CollectableBase>::value));
        EZ_ASSERT(refIndex.isValid(), "");

        auto memory = refIndex.isStatic() ? m_staticAllocations.getMemory()
                                          : m_dynamicAllocations.getMemory();

        auto pInstance = (T*)&memory[refIndex.m_uiIndex];
        auto pCollectable = static_cast<CollectableBase*>(pInstance);

        return refIndex == pCollectable->getRefIndex() ? pInstance : nullptr;
    }
}
