
inline
ezResult
lcpp::LispObjectStack::pop()
{
    if (m_stack.IsEmpty())
    {
        return EZ_FAILURE;
    }

    m_stack.PopBack();
    return EZ_SUCCESS;
}

inline
void
lcpp::LispObjectStack::push(Ptr<LispObject> pObject)
{
    m_stack.PushBack(pObject);
}

inline
ezUInt32
lcpp::LispObjectStack::top()
{
    if(isEmpty()) { return 0; }
    return size() - 1;
}

inline
ezUInt32
lcpp::LispObjectStack::size()
{
    return m_stack.GetCount();
}

inline
bool
lcpp::LispObjectStack::isEmpty()
{
    return m_stack.IsEmpty();
}

void lcpp::LispObjectStack::clear()
{
    m_stack.Clear();
}

inline
ezString&
lcpp::LispObjectStack::name()
{
    return m_name;
}

inline
const ezString&
lcpp::LispObjectStack::name() const
{
    return m_name;
}

inline
void
lcpp::LispObjectStack::name(const ezString& name)
{
    m_name = name;
}

inline
ezUInt32
lcpp::LispObjectStack::convertToAbsolute(ezInt32 relativeIndex)
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
