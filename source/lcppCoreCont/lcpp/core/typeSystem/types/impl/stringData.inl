
namespace lcpp
{
    namespace str
    {
        EZ_FORCE_INLINE
        const String& Data::getValue() const
        {
            return reinterpret_cast<const String&>(m_pRawData);
        }

        EZ_FORCE_INLINE
        String& Data::getValue()
        {
            return reinterpret_cast<String&>(m_pRawData);
        }

        EZ_FORCE_INLINE
        void Data::setValue(const String& newString)
        {
            getValue() = newString;
        }
    }
}
