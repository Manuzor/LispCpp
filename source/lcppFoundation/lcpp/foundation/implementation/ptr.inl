
template<typename T>
inline
lcpp::Ptr<T>::Ptr() :
    m_pPtr(nullptr)
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
lcpp::Ptr<T>::Ptr(T* pPtr) :
    m_pPtr(pPtr)
{
}


template<typename T>
template<typename T_Other>
lcpp::Ptr<T>::Ptr(const Ptr<T_Other>& pOther)
{
    *this = pOther.cast<T>();
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
lcpp::Ptr<T>::isNull() const
{
    return m_pPtr == nullptr;
}

template<typename T>
inline
lcpp::Ptr<T>::operator bool() const
{
    return !isNull();
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
    return ptr.isNull();
}

template<typename T_Rhs, typename T_Lhs>
inline
lcpp::Ptr<T_Rhs>
lcpp::cast(const Ptr<T_Lhs>& lhs)
{
    return static_cast<T_Rhs*>(lhs.get());
}

template<typename T_Lhs, typename T_Rhs>
inline
bool
lcpp::operator == (Ptr<T_Lhs> lhs, Ptr<T_Rhs> rhs)
{
    return lhs.get() == rhs.get();
}

template<typename T_Lhs, typename T_Rhs>
inline
bool
lcpp::operator != (Ptr<T_Lhs> lhs, Ptr<T_Rhs> rhs)
{
    return lhs.get() != rhs.get();
}

template<typename T>
ezUInt32 ezHashHelper<lcpp::Ptr<T>>::Hash(lcpp::Ptr<T> value)
{
    return ezHashHelper<T*>::Hash(value.get());
}

template<typename T>
bool ezHashHelper<lcpp::Ptr<T>>::Equal(lcpp::Ptr<T> a, lcpp::Ptr<T> b)
{
    return ezHashHelper<T*>::Equal(a.get(), b.get());
}
