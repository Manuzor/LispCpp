inline
const SchemeBool&
SchemeObject::is(SchemeType::Enum type) const
{
    bool result = SchemeType::of(*this) == type;
    return SchemeBool::create(result);
}

//////////////////////////////////////////////////////////////////////////

inline
SchemeVoid::SchemeVoid()
{
}

inline
SchemeVoid::~SchemeVoid()
{
}

inline
const SchemeBool&
SchemeVoid::operator==(const SchemeObject& obj) const
{
    return SchemeBool::create(&obj == this);
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
}

inline
SchemeBool::~SchemeBool()
{
}

inline
const SchemeBool&
SchemeBool::create( bool value )
{
    return value ? SCHEME_TRUE : SCHEME_FALSE;
}

inline
const SchemeBool&
SchemeBool::operator==( const SchemeObject& obj ) const
{
    return SCHEME_FALSE;
}

inline
ezString
SchemeBool::toString() const
{
    if (this == &SCHEME_TRUE)
    {
        return "#t";
    }
    else if (this == &SCHEME_TRUE)
    {
        return "#f";
    }
    else
    {
        throw exceptions::InvalidOperation("There cannot be another instance of SchemeBool besides g_true and g_false!");
    }
}

inline
SchemeBool::operator
bool() const
{
    EZ_ASSERT(this == &SCHEME_TRUE || this == &SCHEME_FALSE, "There cannot be another instance of SchemeBool other that SCHEME_TRUE and SCHEME_FALSE!");
    return &SCHEME_TRUE == this;
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
SchemeCons::operator==( const SchemeObject& obj ) const
{
    return SCHEME_FALSE;
}

inline
ezString
SchemeCons::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("(%s", m_car->toString().GetData());

    if (m_cdr->is(SchemeType::Cons))
    {
    }

    builder.Append(")");
    return builder.GetData();
}

//////////////////////////////////////////////////////////////////////////

inline
SchemeNil::SchemeNil()
{
}

inline
SchemeNil::~SchemeNil()
{
}

inline
const SchemeBool&
SchemeNil::operator==(const SchemeObject& obj) const
{
    return SchemeBool::create(&obj == this); // identity
}
inline
ezString
SchemeNil::toString() const
{
    return "()";
}


