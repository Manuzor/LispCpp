inline
bool
lcpp::SchemeObject::operator !=(const SchemeObject& rhs) const
{
    return !(*this == rhs);
}

//////////////////////////////////////////////////////////////////////////

template<typename T_Derived, typename T_Base>
inline
const lcpp::Type&
lcpp::SchemeExtend<T_Derived, T_Base>::type() const
{
    return TypeInfo<T_Derived>::type();
}

template<typename T_Derived, typename T_Base>
inline
bool
lcpp::SchemeExtend<T_Derived, T_Base>::is(const Type& type) const
{
    return type.id == TypeInfo<T_Derived>::type().id;
}

template<typename T_Derived, typename T_Base>
inline
void
lcpp::SchemeExtend<T_Derived, T_Base>::copyTo(void* mem) const
{
    new (mem) T_Derived(*static_cast<const T_Derived*>(this));
}
