
namespace lcpp
{
    namespace env
    {
        EZ_FORCE_INLINE
        BindingLocation::BindingLocation(Enum value) :
            m_value(value)
        {
        }

        EZ_FORCE_INLINE
        bool BindingLocation::doesNotExist() const
        {
            return m_value == None;
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
