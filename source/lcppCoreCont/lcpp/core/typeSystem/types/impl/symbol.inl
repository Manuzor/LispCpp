
namespace lcpp
{
    namespace symbol
    {
        inline
        const String& Data::getValue() const
        {
            return reinterpret_cast<const String&>(m_pRawData);
        }

        inline
        String& Data::getValue()
        {
            return reinterpret_cast<String&>(m_pRawData);
        }

        inline
        void Data::setValue(const String& newString)
        {
            getValue() = newString;
        }
    }
}
