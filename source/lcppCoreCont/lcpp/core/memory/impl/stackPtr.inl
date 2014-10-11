
namespace lcpp
{
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
    lcpp::StackPtr<T>::StackPtr(T* ptr) :
        StackPtrBase(ptr)
    {
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::~StackPtr()
    {
        StackPtrBase::~StackPtrBase();
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
