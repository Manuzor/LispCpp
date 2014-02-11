inline
const SchemeBool&
lcpp::convert( bool value )
{
    return value ? SCHEME_TRUE : SCHEME_FALSE;
}

//////////////////////////////////////////////////////////////////////////

inline
SchemeVoid::SchemeVoid()
{
#ifdef _DEBUG
    static ezUInt8 s_instances = 0U;
    EZ_ASSERT(s_instances < 1, "Instantiating SchemeVoid more than once is not allowed!");
    ++s_instances;
#endif // _DEBUG
}

inline
SchemeVoid::~SchemeVoid()
{
}

inline
const SchemeBool&
SchemeVoid::operator ==(const SchemeObject& obj) const
{
    return convert(&obj == this); // identity
}

inline
ezString
SchemeVoid::toString() const
{
    return "#v";
}

//////////////////////////////////////////////////////////////////////////

inline
SchemeBool::SchemeBool()
{
#ifdef _DEBUG
    static ezUInt8 s_instances = 0U;
    EZ_ASSERT(s_instances < 2, "Instantiating SchemeBool more than 2 times is not allowed!");
    ++s_instances;
#endif // _DEBUG
}

inline
SchemeBool::~SchemeBool()
{
}

inline
const SchemeBool&
SchemeBool::operator ==( const SchemeObject& obj ) const
{
    return convert(this == &obj); // identity
}

inline
ezString
SchemeBool::toString() const
{
    EZ_ASSERT(this == &SCHEME_TRUE || this == &SCHEME_FALSE, "There cannot be another instance of SchemeBool other that SCHEME_TRUE and SCHEME_FALSE!");
    return this == &SCHEME_TRUE ? "#t" : "#f";
}

inline
SchemeBool::operator bool() const
{
    EZ_ASSERT(this == &SCHEME_TRUE || this == &SCHEME_FALSE, "There cannot be another instance of SchemeBool other that SCHEME_TRUE and SCHEME_FALSE!");
    return this == &SCHEME_TRUE;
}

//////////////////////////////////////////////////////////////////////////

template<typename NUMBER_TYPE>
inline
SchemeNumber<NUMBER_TYPE>::SchemeNumber(NUMBER_TYPE value) :
    m_value(value)
{
}

template<typename NUMBER_TYPE>
inline
SchemeNumber<NUMBER_TYPE>::~SchemeNumber()
{
}

//////////////////////////////////////////////////////////////////////////

inline
SchemeCons::SchemeCons(const SchemeObject& car, const SchemeObject& cdr) :
    m_car(&car),
    m_cdr(&cdr)
{
    EZ_ASSERT(m_car != this, "Cannot include myself as car or cdr!");
    EZ_ASSERT(m_cdr != this, "Cannot include myself as car or cdr!");
}

inline
SchemeCons::~SchemeCons()
{
}

inline
const SchemeBool&
SchemeCons::operator ==(const SchemeObject& obj) const
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
SchemeCons::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("(%s %s)", m_car->toString().GetData(), m_cdr->toString().GetData());
    return builder.GetData();
}

//////////////////////////////////////////////////////////////////////////

inline
SchemeNil::SchemeNil()
{
#ifdef _DEBUG
    static ezUInt8 s_instances = 0U;
    EZ_ASSERT(s_instances < 1, "Instantiating SchemeNil more than once is not allowed!");
    ++s_instances;
#endif // _DEBUG
}

inline
SchemeNil::~SchemeNil()
{
}

inline
const SchemeBool&
SchemeNil::operator ==(const SchemeObject& obj) const
{
    return convert(&obj == this); // identity
}
inline
ezString
SchemeNil::toString() const
{
    return "()";
}


