inline
const SchemeBool&
SchemeObject::is(SchemeType::Enum type) const
{
    bool result = SchemeType::of(*this) == type;
    return SchemeBool::create(result);
}

//////////////////////////////////////////////////////////////////////////
SCHEME_TYPE_DEFINITION(SchemeVoid);

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
SCHEME_TYPE_DEFINITION(SchemeBool);

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
SCHEME_TYPE_DEFINITION_TPL1(SchemeNumber, NUMBER_TYPE);

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
SCHEME_TYPE_DEFINITION(SchemeCons);

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
SchemeCons::operator==(const SchemeObject& obj) const
{
    if (!obj.is(SchemeType::Cons))
    {
        return SCHEME_FALSE;
    }
    const auto& other = static_cast<const SchemeCons&>(obj);
    return SchemeBool::create(car() == other.car() && cdr() == other.cdr());
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
SCHEME_TYPE_DEFINITION(SchemeNil);

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


