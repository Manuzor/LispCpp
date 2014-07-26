
namespace lcpp
{
    namespace symbol
    {
        inline
        void Data::initialize(String* pSymbol)
        {
            m_pValue = pSymbol;
        }

        inline
        void Data::deinitialize()
        {
            m_pValue = nullptr;
        }

    }
}
