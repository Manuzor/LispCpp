
namespace lcpp
{
    namespace time
    {
        EZ_FORCE_INLINE
        ezTime& Data::getTime()
        {
            return reinterpret_cast<ezTime&>(m_time);
        }

        EZ_FORCE_INLINE
        const ezTime& Data::getTime() const
        {
            return reinterpret_cast<const ezTime&>(m_time);
        }

        EZ_FORCE_INLINE
        void Data::setTime(const ezTime& value)
        {
            reinterpret_cast<ezTime&>(m_time) = value;
        }
    }
}
