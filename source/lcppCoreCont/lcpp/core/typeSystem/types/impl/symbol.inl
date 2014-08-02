
namespace lcpp
{
    namespace symbol
    {
        EZ_FORCE_INLINE
        Ptr<LispObject> create(ezUInt32 character)
        {
            auto characterString = ezStringBuilder();
            characterString.Append(character);
            return create(characterString);
        }

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
