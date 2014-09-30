
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
    StackPtr<LispObject> InsanceTable<T_Key>::get(const T_Key& key)
    {
        StackPtr<LispObject> pResult;
        LispObject* pRawResult;
        if(!m_table.TryGetValue(key, pRawResult))
        {
            pResult = (*m_pFunctor_createNew)(key);
            m_table[key] = pResult.get();
        }
        else
        {
            pResult = pRawResult;
        }
        EZ_ASSERT(pResult, "The result should never be a nullptr!");

        return pResult;
    }
}
