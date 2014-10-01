
namespace lcpp
{
    namespace env
    {
        EZ_FORCE_INLINE
        Ptr<LispObject>& Data::getName()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pName);
        }

        EZ_FORCE_INLINE
        Ptr<LispObject>& Data::getParent()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pParent);
        }

        EZ_FORCE_INLINE
        HashTable& Data::getTable()
        {
            return m_table;
        }

    };
}
