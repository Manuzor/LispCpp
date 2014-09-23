
namespace lcpp
{
    template<typename T_Data>
    EZ_FORCE_INLINE
    T_Data& lcpp::LispObject::getData()
    {
        auto pData = reinterpret_cast<T_Data*>(m_data);
        return *pData;
    }
}
