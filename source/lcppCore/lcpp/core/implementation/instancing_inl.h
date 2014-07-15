
template<typename T_Key, typename T_Value>
inline
lcpp::InsanceTable<T_Key, T_Value>::InsanceTable() :
    m_table(LispRuntime::instance()->allocator().get())
{
}

template<typename T_Key, typename T_Value>
inline
lcpp::Ptr<T_Value>
lcpp::InsanceTable<T_Key, T_Value>::get(const T_Key& key)
{
    Ptr<T_Value> pResult;
    if(!m_table.TryGetValue(key, pResult))
    {
        pResult = T_Value::createNew(key);
        m_table[key] = pResult;
    }
    EZ_ASSERT(pResult, "The result should never be a nullptr!");

    return pResult;
}
