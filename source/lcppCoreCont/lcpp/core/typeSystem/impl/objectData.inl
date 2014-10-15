
namespace lcpp
{
    template<typename T_Data>
    EZ_FORCE_INLINE
    T_Data& lcpp::LispObject::getData()
    {
        auto pData = reinterpret_cast<T_Data*>(this + 1);
        return *pData;
    }
}
