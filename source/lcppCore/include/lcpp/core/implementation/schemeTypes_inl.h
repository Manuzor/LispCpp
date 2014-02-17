inline
const lcpp::SchemeBool&
lcpp::convert( bool value )
{
    return value ? SCHEME_TRUE : SCHEME_FALSE;
}

//////////////////////////////////////////////////////////////////////////

inline
lcpp::SchemeVoid::SchemeVoid()
{
#ifdef _DEBUG
    static ezUInt8 s_instances = 0U;
    EZ_ASSERT(s_instances < 1, "Instantiating SchemeVoid more than once is not allowed!");
    ++s_instances;
#endif // _DEBUG
}

inline
lcpp::SchemeVoid::~SchemeVoid()
{
}

inline
const lcpp::SchemeBool&
lcpp::SchemeVoid::operator ==(const SchemeObject& obj) const
{
    return convert(&obj == this); // identity
}

inline
ezString
lcpp::SchemeVoid::toString() const
{
    return "#v";
}

//////////////////////////////////////////////////////////////////////////

inline
lcpp::SchemeBool::SchemeBool()
{
#ifdef _DEBUG
    static ezUInt8 s_instances = 0U;
    EZ_ASSERT(s_instances < 2, "Instantiating SchemeBool more than 2 times is not allowed!");
    ++s_instances;
#endif // _DEBUG
}

inline
lcpp::SchemeBool::~SchemeBool()
{
}

inline
const lcpp::SchemeBool&
lcpp::SchemeBool::operator ==( const SchemeObject& obj ) const
{
    return convert(this == &obj); // identity
}

inline
ezString
lcpp::SchemeBool::toString() const
{
    EZ_ASSERT(this == &SCHEME_TRUE || this == &SCHEME_FALSE, "There cannot be another instance of SchemeBool other that SCHEME_TRUE and SCHEME_FALSE!");
    return this == &SCHEME_TRUE ? "#t" : "#f";
}

inline
lcpp::SchemeBool::operator bool() const
{
    EZ_ASSERT(this == &SCHEME_TRUE || this == &SCHEME_FALSE, "There cannot be another instance of SchemeBool other that SCHEME_TRUE and SCHEME_FALSE!");
    return this == &SCHEME_TRUE;
}

//////////////////////////////////////////////////////////////////////////

template<typename NUMBER_TYPE>
inline
lcpp::SchemeNumber_t<NUMBER_TYPE>::SchemeNumber_t(NUMBER_TYPE value) :
    m_value(value)
{
}

template<typename NUMBER_TYPE>
inline
lcpp::SchemeNumber_t<NUMBER_TYPE>::~SchemeNumber_t()
{
}

template<typename NUMBER_TYPE>
inline
const lcpp::SchemeBool&
lcpp::SchemeNumber_t<NUMBER_TYPE>::operator ==(const SchemeObject& obj) const
{
    if (obj.is(SchemeType::Number))
    {
        return convert(*this == static_cast<const SchemeNumber_t<NUMBER_TYPE>&>(obj));
    }
    return SCHEME_FALSE;
}

template<typename NUMBER_TYPE>
inline
ezString
lcpp::SchemeNumber_t<NUMBER_TYPE>::toString() const
{
    EZ_ASSERT(false, "Not Implemented!");
    return "";
}

template<typename NUMBER_TYPE>
inline
const lcpp::SchemeBool&
lcpp::SchemeNumber_t<NUMBER_TYPE>::operator ==(const SchemeNumber_t<NUMBER_TYPE>& other) const
{
    return convert(m_value == other.m_value);
}

//////////////////////////////////////////////////////////////////////////

inline
lcpp::SchemeCons::SchemeCons(const SchemeObject& car, const SchemeObject& cdr) :
    m_car(&car),
    m_cdr(&cdr)
{
    EZ_ASSERT(m_car != this, "Cannot include myself as car or cdr!");
    EZ_ASSERT(m_cdr != this, "Cannot include myself as car or cdr!");
}

inline
lcpp::SchemeCons::~SchemeCons()
{
}

inline
const lcpp::SchemeBool&
lcpp::SchemeCons::operator ==(const SchemeObject& obj) const
{
    if (!obj.is(SchemeType::Cons))
    {
        return SCHEME_FALSE;
    }
    const auto& other = static_cast<const SchemeCons&>(obj);
    return convert(car() == other.car() && cdr() == other.cdr());
}

inline
ezString
lcpp::SchemeCons::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("(%s %s)", m_car->toString().GetData(), m_cdr->toString().GetData());
    return builder.GetData();
}

//////////////////////////////////////////////////////////////////////////

inline
lcpp::SchemeNil::SchemeNil()
{
#ifdef _DEBUG
    static ezUInt8 s_instances = 0U;
    EZ_ASSERT(s_instances < 1, "Instantiating SchemeNil more than once is not allowed!");
    ++s_instances;
#endif // _DEBUG
}

inline
lcpp::SchemeNil::~SchemeNil()
{
}

inline
const lcpp::SchemeBool&
lcpp::SchemeNil::operator ==(const SchemeObject& obj) const
{
    return convert(&obj == this); // identity
}
inline
ezString
lcpp::SchemeNil::toString() const
{
    return "()";
}


