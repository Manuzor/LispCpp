
namespace lcpp
{
    namespace env
    {
        EZ_FORCE_INLINE
        Ptr<LispObject> Data::getName()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pName);
        }

        EZ_FORCE_INLINE
        Ptr<LispObject> Data::getParent()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pParent);
        }

        EZ_FORCE_INLINE
        HashTable& Data::getTable()
        {
            return reinterpret_cast<HashTable&>(m_table);
        }

        EZ_FORCE_INLINE
        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            return getQualifiedName(pObject);
        }

        EZ_FORCE_INLINE
        BindingLocation::BindingLocation(Enum value) :
            m_value(value)
        {
        }

        EZ_FORCE_INLINE
        bool BindingLocation::locally() const
        {
            return m_value == Local;
        }

        EZ_FORCE_INLINE
        bool BindingLocation::inParent() const
        {
            return m_value == Parent;
        }

        EZ_FORCE_INLINE
        BindingLocation::operator bool() const
        {
            return m_value != None;
        }
    };
}
