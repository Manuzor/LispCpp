inline
bool
lcpp::SchemeObject::operator !=(const SchemeObject& rhs) const
{
    return !(*this == rhs);
}

//////////////////////////////////////////////////////////////////////////

template<typename Derived, typename Base>
inline
const lcpp::Type&
lcpp::SchemeExtend<Derived, Base>::type() const
{
    return TypeInfo<Derived>::type();
}

template<typename Derived, typename Base>
inline
bool
lcpp::SchemeExtend<Derived, Base>::is(const Type& type) const
{
    return type.id == TypeInfo<Derived>::type().id;
}

template<typename Derived, typename Base>
inline
void
lcpp::SchemeExtend<Derived, Base>::copyTo(void* mem) const
{
    new (mem) Derived(*static_cast<const Derived*>(this));
}   
