
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
        void Data::destroy()
        {
            m_pValue = nullptr;
        }

    }
}
