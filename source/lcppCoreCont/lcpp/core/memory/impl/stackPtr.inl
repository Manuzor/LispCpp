
namespace lcpp
{
    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr()
    {
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(const StackPtr& rhs)
    {
        m_ptr = rhs.m_ptr;
        addToGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(StackPtr&& rhs)
    {
        rhs.removeFromGc();
        m_ptr = std::move(rhs.m_ptr);
        addToGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(Ptr<T> ptr)
    {
        m_ptr = ptr;
        addToGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    lcpp::StackPtr<T>::StackPtr(T* ptr)
    {
        m_ptr = ptr;
        addToGc();
    }

    template<typename T>
    template<typename T_Other>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(const StackPtr<T_Other>& other)
    {
        m_ptr = other.get();
        addToGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::~StackPtr()
    {
        removeFromGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(const StackPtr& toCopy)
    {
        StackPtr<T> copy(toCopy);

        using namespace std;
        swap(m_ptr, copy.m_ptr);
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(StackPtr&& toMove)
    {
        removeFromGc();
        toMove.removeFromGc();
        m_ptr = std::move(toMove.m_ptr);
        addToGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(Ptr<T> pPtr)
    {
        removeFromGc();
        m_ptr = pPtr;
        addToGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(T* pPtr)
    {
        removeFromGc();
        m_ptr = pPtr;
        addToGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    T* StackPtr<T>::operator ->() const
    {
        return get();
    }

    template<typename T>
    EZ_FORCE_INLINE
    T& StackPtr<T>::operator *() const
    {
        return *get();
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    T* StackPtr<T>::get() const
    {
        return static_cast<T*>(m_ptr.get());
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    bool StackPtr<T>::isNull() const
    {
        return m_ptr.isNull();
    }

    template<typename T>
    template<typename T_Other>
    EZ_FORCE_INLINE
    StackPtr<T_Other> StackPtr<T>::cast() const
    {
        return m_ptr.cast<T_Other>();
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::operator bool() const
    {
        return !isNull();
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::addToGc() const
    {
        if(!isNull())
            m_ptr->getGarbageCollector()->addStackPtr(this);
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::removeFromGc() const
    {
        if(!isNull())
            m_ptr->getGarbageCollector()->removeStackPtr(this);
    }
}

template<typename T>
ezUInt32 ezHashHelper<lcpp::StackPtr<T>>::Hash(lcpp::StackPtr<T> value)
{
    return ezHashHelper<T*>::Hash(value.get());
}

template<typename T>
bool ezHashHelper<lcpp::StackPtr<T>>::Equal(lcpp::StackPtr<T> a, lcpp::StackPtr<T> b)
{
    return ezHashHelper<T*>::Equal(a.get(), b.get());
}
