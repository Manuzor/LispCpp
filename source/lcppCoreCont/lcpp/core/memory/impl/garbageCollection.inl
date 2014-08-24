
namespace lcpp
{
    EZ_FORCE_INLINE
    Ptr<LispObject>::Ptr() :
        m_pPtr(nullptr)
    {
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::Ptr(const Ptr& rhs) :
        m_pPtr(rhs.m_pPtr)
    {
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::Ptr(Ptr&& rhs) :
        m_pPtr(rhs.m_pPtr)
    {
        rhs.m_pPtr = nullptr;
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::Ptr(LispObject* pPtr) :
        m_pPtr(pPtr)
    {
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::~Ptr()
    {
        m_pPtr = nullptr;
    }

    EZ_FORCE_INLINE
    void Ptr<LispObject>::operator =(const Ptr& toCopy)
    {
        Ptr<LispObject> copy = toCopy;

        std::swap(m_pPtr, copy.m_pPtr);
    }
    
    EZ_FORCE_INLINE
    void Ptr<LispObject>::operator =(Ptr&& toMove)
    {
        m_pPtr = toMove.m_pPtr;
        toMove.m_pPtr = nullptr;
    }
    
    EZ_FORCE_INLINE
    void Ptr<LispObject>::operator =(LispObject* pPtr)
    {
        m_pPtr = pPtr;
    }
    
    EZ_FORCE_INLINE
    LispObject* Ptr<LispObject>::get() const
    {
        return m_pPtr;
    }
    
    EZ_FORCE_INLINE
    bool Ptr<LispObject>::isNull() const
    {
        return m_pPtr == nullptr;
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::operator bool() const
    {
        return !isNull();
    }
    
    EZ_FORCE_INLINE
    LispObject* Ptr<LispObject>::operator ->() const
    {
        EZ_ASSERT(m_pPtr, "Accessing nullptr!");
        return m_pPtr;
    }
    
    EZ_FORCE_INLINE
    LispObject& Ptr<LispObject>::operator *() const
    {
        EZ_ASSERT(m_pPtr, "Dereferencing nullptr!");
        return *m_pPtr;
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

    EZ_FORCE_INLINE
    ezAllocatorBase* GarbageCollector::getAllocator()
    {
        return this;
    }
}
