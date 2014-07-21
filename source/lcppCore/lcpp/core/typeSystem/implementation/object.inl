inline
bool
lcpp::LispObject::operator !=(const LispObject& rhs) const
{
    return !(*this == rhs);
}

template<typename T_Other>
inline
bool
lcpp::LispObject::is() const
{
    return type() == T_Other::typeInfo();
}

//////////////////////////////////////////////////////////////////////////

template<typename T_Derived, typename T_Base>
inline
const lcpp::Type&
lcpp::LispExtend<T_Derived, T_Base>::type() const
{
    return T_Derived::typeInfo();
}

template<typename T_Derived, typename T_Base>
inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispExtend<T_Derived, T_Base>::copy() const
{
    return LCPP_NEW(pAllocator, T_Derived)(static_cast<const T_Derived&>(*this));
}

inline
lcpp::SourcePosition&
lcpp::LispObject::sourcePosition()
{
    return m_sourcePosition;
}

inline
const lcpp::SourcePosition&
lcpp::LispObject::sourcePosition() const
{
    return m_sourcePosition;
}

inline
void
lcpp::LispObject::sourcePosition(const lcpp::SourcePosition& sourcePosition)
{
    m_sourcePosition = sourcePosition;
}
