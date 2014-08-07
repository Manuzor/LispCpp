
template<typename T_Element, ezUInt32 N_StaticSize>
inline
lcpp::StackBase<T_Element, N_StaticSize>::StackBase(Ptr<ezAllocatorBase> pAllocator) :
    m_pAllocator(pAllocator),
    m_stack(pAllocator.get()),
    m_nilElement()
{
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
ezResult
lcpp::StackBase<T_Element, N_StaticSize>::pop()
{
    if (m_stack.IsEmpty())
    {
        return EZ_FAILURE;
    }

    m_stack.PopBack();
    return EZ_SUCCESS;
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
ezResult
lcpp::StackBase<T_Element, N_StaticSize>::pop(ezInt32 relativeIndex)
{
    if (m_stack.IsEmpty())
    {
        return EZ_FAILURE;
    }

    auto index = convertToAbsolute(relativeIndex);

    m_stack.RemoveAt(index);

    return EZ_SUCCESS;
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
void
lcpp::StackBase<T_Element, N_StaticSize>::push(const T_Element& newElement)
{
    m_stack.PushBack(newElement);
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
T_Element&
lcpp::StackBase<T_Element, N_StaticSize>::get(ezInt32 relativeIndex)
{
    return const_cast<T_Element&>(const_cast<const StackBase* const>(this)->get(relativeIndex));
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
const T_Element&
lcpp::StackBase<T_Element, N_StaticSize>::get(ezInt32 relativeIndex) const
{
    if(isEmpty()) { return m_nilElement; }

    auto index = convertToAbsolute(relativeIndex);
    if(index > top())
    {
        // Invalid index (out of bounds)
        return m_nilElement;
    }

    return m_stack[index];
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
ezUInt32
lcpp::StackBase<T_Element, N_StaticSize>::top() const
{
    if(isEmpty()) { return 0; }
    return size() - 1;
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
ezUInt32
lcpp::StackBase<T_Element, N_StaticSize>::size() const
{
    return m_stack.GetCount();
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
bool
lcpp::StackBase<T_Element, N_StaticSize>::isEmpty() const
{
    return m_stack.IsEmpty();
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
void
lcpp::StackBase<T_Element, N_StaticSize>::clear()
{
    m_stack.Clear();
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
void
lcpp::StackBase<T_Element, N_StaticSize>::setNilElement(const T_Element& newNilElement)
{
    m_nilElement = newNilElement;
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
const T_Element&
lcpp::StackBase<T_Element, N_StaticSize>::getNilElement() const
{
    return m_nilElement;
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
T_Element&
lcpp::StackBase<T_Element, N_StaticSize>::getNilElement()
{
    return m_nilElement;
}

template<typename T_Element, ezUInt32 N_StaticSize>
inline
ezUInt32
lcpp::StackBase<T_Element, N_StaticSize>::convertToAbsolute(ezInt32 relativeIndex) const
{
    if (relativeIndex >= 0)
    {
        // The index is positive, which means it must already be absolute.
        return ezUInt32(relativeIndex);
    }
    
    // Since the relativeIndex must be smaller than 0 at this point,
    // we add it to the current size to get the absolute index.
    auto absoluteIndex = size() + relativeIndex;
    return absoluteIndex;
}
