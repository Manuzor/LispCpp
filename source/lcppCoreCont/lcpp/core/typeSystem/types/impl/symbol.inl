
namespace lcpp
{
    namespace symbol
    {
        inline
        void Data::initialize(const String& symbol)
        {
            setString(symbol);
        }

        inline
        void Data::destroy()
        {
            getString().~String();
        }

        inline
        const String& Data::getString() const
        {
            return reinterpret_cast<const String&>(m_pRawData);
        }

        inline
        String& Data::getString()
        {
            return reinterpret_cast<String&>(m_pRawData);
        }

        inline
        void Data::setString(const String& newString)
        {
            getString() = newString;
        }
    }
}
