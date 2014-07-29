
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

        inline
        BindingLocation::BindingLocation(Enum value) :
            m_value(value)
        {
        }

        inline
        bool BindingLocation::locally() const
        {
            return m_value == Local;
        }

        inline
        bool BindingLocation::inParent() const
        {
            return m_value == Parent;
        }

        inline
        BindingLocation::operator bool() const
        {
            return m_value != None;
        }
    };
}
