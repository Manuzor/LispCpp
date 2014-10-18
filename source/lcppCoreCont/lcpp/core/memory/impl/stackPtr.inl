
namespace lcpp
{
    inline
    StackPtrBase::StackPtrBase(CollectableBase* ptr) :
        m_uiIndex(s_uiNextIndex++)
    {
        EZ_ASSERT(s_uiNextIndex < NumMaxStackPtrs, "Maximum number of supported stack ptrs reached.");
        s_ptrTable[m_uiIndex] = ptr;
        LCPP_InDebug( m_pLastLookup = ptr; );
    }

    inline
    StackPtrBase::StackPtrBase(const StackPtrBase& toCopy) :
        m_uiIndex(s_uiNextIndex++)
    {
        EZ_ASSERT(s_uiNextIndex < NumMaxStackPtrs, "Maximum number of supported stack ptrs reached.");
        *this = toCopy;
    }

    inline
    StackPtrBase::~StackPtrBase()
    {
        auto uiExepectedIndex = --s_uiNextIndex;
        EZ_ASSERT(m_uiIndex == uiExepectedIndex, "Destructing in wrong order!");
        LCPP_InDebug( m_pLastLookup = nullptr; );
    }

    inline
    void StackPtrBase::operator=(const StackPtrBase& toCopy)
    {
        *this = s_ptrTable[toCopy.m_uiIndex];
    }

    inline
    void StackPtrBase::operator=(CollectableBase* ptr)
    {
        EZ_ASSERT(ptr != nullptr, "");
        s_ptrTable[m_uiIndex] = ptr;
        LCPP_InDebug( m_pLastLookup = ptr; );
    }

    inline
    CollectableBase* StackPtrBase::get()
    {
        LCPP_InDebug(
            auto pResult = s_ptrTable[m_uiIndex];
            EZ_ASSERT(pResult != nullptr, "");
            m_pLastLookup = pResult;
            return pResult;
        );
        LCPP_InNonDebug( return s_ptrTable[m_uiIndex]; );
    }

    inline
    bool StackPtrBase::isNull()
    {
        auto ptr = s_ptrTable[m_uiIndex];
        LCPP_InDebug( m_pLastLookup = ptr; );
        return ptr == nullptr;
    }

    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(StackPtr& rhs) :
        StackPtrBase(rhs.get())
    {
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(Ptr<T> ptr) :
        StackPtrBase(ptr.get())
    {
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(T* ptr) :
        StackPtrBase(ptr)
    {
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(StackPtr& toCopy)
    {
        StackPtrBase::operator=(static_cast<StackPtrBase&>(toCopy).get());
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(Ptr<T> pPtr)
    {
        StackPtrBase::operator=(static_cast<CollectableBase*>(pPtr.get()));
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(T* pPtr)
    {
        StackPtrBase::operator=(static_cast<CollectableBase*>(pPtr));
    }

    template<typename T>
    EZ_FORCE_INLINE
    T* StackPtr<T>::operator ->()
    {
        return static_cast<T*>(StackPtrBase::get());
    }

    template<typename T>
    EZ_FORCE_INLINE
    T& StackPtr<T>::operator *()
    {
        return *static_cast<T*>(StackPtrBase::get());
    }

    template<typename T>
    EZ_FORCE_INLINE
    T* StackPtr<T>::get()
    {
        return static_cast<T*>(StackPtrBase::get());
    }

    template<typename T>
    EZ_FORCE_INLINE
    bool StackPtr<T>::isNull()
    {
        return StackPtrBase::isNull();
    }

    template<typename T>
    template<typename T_Other>
    EZ_FORCE_INLINE
    StackPtr<T_Other> StackPtr<T>::cast()
    {
        return static_cast<T_Other*>(get());
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::operator bool()
    {
        return !isNull();
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
