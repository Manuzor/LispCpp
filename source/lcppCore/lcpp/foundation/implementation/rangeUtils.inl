
template<typename T, typename Derived>
T*
begin( ezArrayBase<T, Derived>& arr )
{
    return arr.GetCount() > 0 ? &arr[0] : nullptr;
}

template<typename T, typename Derived>
const T*
begin( const ezArrayBase<T, Derived>& arr )
{
    return arr.GetCount() > 0 ? &arr[0] : nullptr;
}

template<typename T, typename Derived>
T*
end( ezArrayBase<T, Derived>& arr )
{
    const ezUInt32 count = arr.GetCount();

    if (count == 0) return nullptr;

    T* val = &arr[count - 1];
    return val + 1;
}

template<typename T, typename Derived>
const T*
end( const ezArrayBase<T, Derived>& arr )
{
    const ezUInt32 count = arr.GetCount();

    if (count == 0) return nullptr;

    T* val = &arr[count - 1];
    return val + 1;
}
