
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

    //////////////////////////////////////////////////////////////////////////
    
    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::createStatic()
    {
        LCPP_LogBlock("GarbageCollector::createStatic");

        auto pInstance = EZ_NEW(m_pAllocator, T);
        pInstance->setGarbageCollector(this);

        return pInstance;
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::create()
    {
        LCPP_LogBlock("GarbageCollector::create");
        
        EZ_CHECK_AT_COMPILETIME((std::is_convertible<T, CollectableBase>::value));

        LCPP_LogBlock("GarbageCollector::basicCreate");
        
        T* pInstance = nullptr;

        while(true)
        {
            auto result = m_memory.allocate(pInstance);

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

        return pInstance;
    }

}
