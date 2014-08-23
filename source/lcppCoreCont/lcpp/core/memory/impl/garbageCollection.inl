
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
}
