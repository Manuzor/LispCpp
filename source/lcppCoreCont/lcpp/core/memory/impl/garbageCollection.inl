
namespace lcpp
{
    EZ_FORCE_INLINE
    RefIndex CollectableBase::getRefIndex() const
    {
        return m_refIndex;
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
        m_left.SetCountUninitialized(uiCount);
        m_right.SetCountUninitialized(uiCount);
    }
    
    EZ_FORCE_INLINE
    void GarbageCollector::DualArrayWrapper::AddCountUninitialized(ezUInt32 uiCount)
    {
        SetCountUninitialized(GetCount() + uiCount);
    }
    
    EZ_FORCE_INLINE
    ezUInt32 GarbageCollector::DualArrayWrapper::GetCount()
    {
        EZ_ASSERT(m_left.GetCount() == m_right.GetCount(), "");
        return m_left.GetCount();
    }
    
    EZ_FORCE_INLINE
    void GarbageCollector::DualArrayWrapper::EnsureRangeIsValid(ezUInt32 uiStartIndex, ezUInt32 uiCount)
    {
        auto uiTargetCount = uiStartIndex + uiCount;

        EZ_ASSERT(uiTargetCount >= uiStartIndex, "Unsigned wrap-around!");

        if (uiTargetCount > GetCount())
        {
            SetCountUninitialized(uiTargetCount);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    
    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::create()
    {
        EZ_CHECK_AT_COMPILETIME((std::is_convertible<T, CollectableBase>::value));

        RefIndex refIndex;
        refIndex.m_uiIndex = m_uiAllocationIndex;
        
        T* pInstance(nullptr);
        pInstance = (T*)Allocate(sizeof(T), EZ_ALIGNMENT_OF(T));
        
        new (pInstance) T();
        pInstance->m_refIndex = refIndex;

        return pInstance;
    }

    EZ_FORCE_INLINE
    ezAllocatorBase* GarbageCollector::getAllocator()
    {
        return this;
    }
}
