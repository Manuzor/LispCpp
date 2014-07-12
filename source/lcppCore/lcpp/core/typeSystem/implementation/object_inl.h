inline
bool
lcpp::LispObject::operator !=(const LispObject& rhs) const
{
    return !(*this == rhs);
}

template<typename T_Other>
inline
bool lcpp::LispObject::is() const
{
    return type().id == TypeInfo<T_Other>::type().id;
}

//////////////////////////////////////////////////////////////////////////

template<typename T_Derived, typename T_Base>
inline
const lcpp::Type&
lcpp::LispExtend<T_Derived, T_Base>::type() const
{
    return TypeInfo<T_Derived>::type();
}

template<typename T_Derived, typename T_Base>
inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispExtend<T_Derived, T_Base>::clone(ezAllocatorBase* pAllocator) const
{
    return LCPP_NEW(pAllocator, T_Derived)(static_cast<const T_Derived&>(*this));
}
