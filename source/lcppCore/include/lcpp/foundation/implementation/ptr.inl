
template<typename T>
inline
lcpp::Ptr<T>::Ptr() :
    m_pPtr(nullptr)
{
}

template<typename T>
inline
lcpp::Ptr<T>::Ptr(T* pPtr) :
    m_pPtr(pPtr)
{
}

template<typename T>
inline
lcpp::Ptr<T>::Ptr(const Ptr& rhs) :
    m_pPtr(rhs.m_pPtr)
{
}

template<typename T>
inline
lcpp::Ptr<T>::Ptr(Ptr&& rhs) :
    m_pPtr(rhs.m_pPtr)
{
    rhs.m_pPtr = nullptr;
}

template<typename T>
inline
lcpp::Ptr<T>::~Ptr()
{
    m_pPtr = nullptr;
}

template<typename T>
inline
void
lcpp::Ptr<T>::operator =(const Ptr& toCopy)
{
    Ptr<T> copy = toCopy;

    std::swap(m_pPtr, copy.m_pPtr);
}

template<typename T>
inline
void
lcpp::Ptr<T>::operator =(Ptr&& toMove)
{
    m_pPtr = toMove.m_pPtr;
    toMove.m_pPtr = nullptr;
}

template<typename T>
inline
void
lcpp::Ptr<T>::operator =(T* pPtr)
{
    m_pPtr = pPtr;
}

template<typename T>
inline
T*
lcpp::Ptr<T>::get() const
{
    return m_pPtr;
}

template<typename T>
inline
bool
lcpp::Ptr<T>::valid() const
{
    return m_pPtr != nullptr;
}

template<typename T>
inline
lcpp::Ptr<T>::operator bool() const
{
    return valid();
}


template<typename T>
template<typename T_Other>
lcpp::Ptr<T_Other>
lcpp::Ptr<T>::cast() const
{
    return lcpp::cast<T_Other>(*this);
}

template<typename T>
inline
T*
lcpp::Ptr<T>::operator ->() const
{
    return m_pPtr;
}

template<typename T>
inline
T&
lcpp::Ptr<T>::operator *() const
{
    return *m_pPtr;
}

// Free functions

template<typename T>
inline
bool
lcpp::operator !(const Ptr<T>& ptr)
{
    return !(ptr.valid());
}

template<typename T_Rhs, typename T_Lhs>
inline
lcpp::Ptr<T_Rhs>
lcpp::cast(const Ptr<T_Lhs>& lhs)
{
    return static_cast<T_Rhs*>(lhs.get());
}
