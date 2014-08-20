
namespace lcpp
{
    namespace stream
    {
        inline
        ezStringIterator& Data::getIterator()
        {
            return reinterpret_cast<ezStringIterator&>(m_iterator);
        }
    }
}
