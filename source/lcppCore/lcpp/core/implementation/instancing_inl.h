
template<typename T_Key, typename T_Value>
inline
lcpp::InsanceTable::InsanceTable(Ptr<LispRuntime> pRuntime) :
    m_table(pRuntime->allocator().get())
{
}

template<typename T_Key, typename T_Value>
inline
lcpp::Ptr<T_Value>
lcpp::InsanceTable::get(const T_Key& key)
{
    Ptr<T_Value> pResult;
    if(!m_table.TryGetValue(key, pResult))
    {
        pResult = T_Value::createInstance(key);
        m_table[key] = pResult;
    }
    EZ_ASSERT(pResult, "The result should never be a nullptr!");

    return pResult;
}
