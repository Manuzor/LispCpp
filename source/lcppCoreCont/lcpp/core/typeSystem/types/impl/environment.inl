
namespace lcpp
{
    namespace env
    {
        inline
        Ptr<LispObject> Data::getName()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pName);
        }

        inline
        Ptr<LispObject> Data::getParent()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pParent);
        }

        inline
        HashTable& Data::getTable()
        {
            return reinterpret_cast<HashTable&>(m_table);
        }
    };
}
