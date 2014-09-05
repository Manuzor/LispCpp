
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
    
    EZ_FORCE_INLINE
    GarbageCollector::DualArrayWrapper::DualArrayWrapper(ezAllocatorBase* pParentAllocator) :
        m_left(pParentAllocator),
        m_right(pParentAllocator)
    {
    }
    
    EZ_FORCE_INLINE
    void GarbageCollector::DualArrayWrapper::SetCountUninitialized(ezUInt32 uiCount)
    {
        LCPP_LogBlock("GarbageCollector::DualArrayWrapper::SetCountUninitialized");
        LCPP_LogVerboseDebugMessage("Setting to %f MiB (%u B)", float(uiCount) / float(1024 * 1024), uiCount);

        m_left.SetCountUninitialized(uiCount);
        m_right.SetCountUninitialized(uiCount);
    }
    
    EZ_FORCE_INLINE
    void GarbageCollector::DualArrayWrapper::AddCountUninitialized(ezUInt32 uiCount)
    {
        LCPP_LogBlock("GarbageCollector::DualArrayWrapper::AddCountUninitialized");
        LCPP_LogVerboseDebugMessage("Adding %u", uiCount);

        SetCountUninitialized(GetCount() + uiCount);
    }
    
    EZ_FORCE_INLINE
    ezUInt32 GarbageCollector::DualArrayWrapper::GetCount()
    {
        EZ_ASSERT(m_left.GetCount() == m_right.GetCount(), "");
        return m_left.GetCount();
    }
    
    EZ_FORCE_INLINE
    GarbageCollector::DualArrayWrapper::Action GarbageCollector::DualArrayWrapper::EnsureRangeIsValid(ezUInt32 uiStartIndex, ezUInt32 uiCount)
    {
        LCPP_LogBlock("GarbageCollector::DualArrayWrapper::EnsureRangeIsValid");

        auto uiTargetCount = uiStartIndex + uiCount;

        EZ_ASSERT(uiTargetCount >= uiStartIndex, "Unsigned wrap-around!");

        if (uiTargetCount > GetCount())
        {
            LCPP_LogVerboseDebugMessage("Target count greater actual count; Resizing.");
            SetCountUninitialized(uiTargetCount);
            return Action::Resized;
        }

        return Action::None;
    }

    EZ_FORCE_INLINE
    GarbageCollector::DualArrayWrapper::Action::Action(Enum value) :
        m_value(value)
    {
    }
    
    EZ_FORCE_INLINE
    bool GarbageCollector::DualArrayWrapper::Action::wasResized() const
    {
        return m_value == Resized;
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
        refIndex.m_uiIndex = m_uiStaticAllocationIndex;
        
        T* pInstance(nullptr);
        pInstance = (T*)allocateStatic(sizeof(T), EZ_ALIGNMENT_OF(T));

        EZ_ASSERT(refIndex.m_uiIndex < m_uiStaticAllocationIndex, "");
        
        new (pInstance) T();
        pInstance->setGarbageCollector(this);
        pInstance->setRefIndex(refIndex);

        return pInstance;
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::create()
    {
        EZ_CHECK_AT_COMPILETIME((std::is_convertible<T, CollectableBase>::value));

        LCPP_LogBlock("GarbageCollector::create");

        RefIndex refIndex;
        refIndex.m_bIsStatic = false;
        refIndex.m_uiHash = 0;
        refIndex.m_uiIndex = m_uiAllocationIndex;
        
        T* pInstance(nullptr);
        pInstance = (T*)allocate(sizeof(T), EZ_ALIGNMENT_OF(T));

        EZ_ASSERT(refIndex.m_uiIndex < m_uiAllocationIndex, "");
        
        new (pInstance) T();
        pInstance->setGarbageCollector(this);
        pInstance->setRefIndex(refIndex);

        return pInstance;
    }

    template<typename T>
    EZ_FORCE_INLINE
    T* GarbageCollector::getPointer(RefIndex refIndex) const
    {
        EZ_ASSERT(refIndex.isValid(), "");
        EZ_ASSERT(refIndex.m_uiIndex <= m_uiAllocationIndex, "");

        Array<byte_t>& targetMemory = refIndex.isStatic() ? m_staticMemory : m_eden;

        auto pMem = &targetMemory[refIndex.m_uiIndex];
        auto pInstance = reinterpret_cast<T*>(pMem);
        auto pCollectable = static_cast<CollectableBase*>(pInstance);

        return refIndex == pCollectable->getRefIndex() ? pInstance : nullptr;
    }
}
