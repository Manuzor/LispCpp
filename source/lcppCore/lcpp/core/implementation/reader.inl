
template<typename T_Object>
inline
lcpp::Ptr<T_Object>
lcpp::Reader::create()
{
    auto pResult = T_Object::create();
    return pResult;
}

template<typename T_Object, typename T_Arg0>
inline
lcpp::Ptr<T_Object>
lcpp::Reader::create(T_Arg0 arg0)
{
    auto pResult = T_Object::create(arg0);
    return pResult;
}

template<typename T_Object, typename T_Arg0, typename T_Arg1>
inline
lcpp::Ptr<T_Object>
lcpp::Reader::create(T_Arg0 arg0, T_Arg1 arg1)
{
    auto pResult = T_Object::create(arg0, arg1);
    return pResult;
}
