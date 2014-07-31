
namespace lcpp
{
    template<typename T_Key>
    inline
    InsanceTable<T_Key>::InsanceTable(createNew_t pFunctor_createNew) :
        m_pFunctor_createNew(pFunctor_createNew),
        m_table(defaultAllocator())
    {
        EZ_ASSERT(m_pFunctor_createNew, "Invalid createNew function pointer.");
    }

    template<typename T_Key>
    inline
    Ptr<LispObject> InsanceTable<T_Key>::get(const T_Key& key)
    {
        auto pResult = Ptr<LispObject>();
        if(!m_table.TryGetValue(key, pResult))
        {
            pResult = (*m_pFunctor_createNew)(key);
            m_table[key] = pResult;
        }
        EZ_ASSERT(pResult, "The result should never be a nullptr!");

        return pResult;
    }
}
