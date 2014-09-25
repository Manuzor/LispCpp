
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
    auto count = arr.GetCount();
    return count > 0 ? (&arr[count - 1] + 1) : nullptr;
}

template<typename T, typename Derived>
const T*
end( const ezArrayBase<T, Derived>& arr )
{
    auto count = arr.GetCount();
    return count > 0 ? (&arr[count - 1] + 1) : nullptr;
}
