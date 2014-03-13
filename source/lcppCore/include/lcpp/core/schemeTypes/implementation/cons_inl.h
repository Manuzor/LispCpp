
inline
lcpp::SchemeCons::SchemeCons() :
    m_car(&SCHEME_NIL),
    m_cdr(&SCHEME_NIL)
{
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeObject& car) :
    m_car(&car),
    m_cdr(&SCHEME_NIL)
{
    EZ_ASSERT(m_car != this, "Cannot include myself as car!");
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeObject& car, const SchemeObject& cdr) :
    m_car(&car),
    m_cdr(&cdr)
{
    EZ_ASSERT(m_car != this, "Cannot include myself as car!");
    EZ_ASSERT(m_cdr != this, "Cannot include myself as cdr!");
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeCons& copy) :
    m_car(copy.m_car),
    m_cdr(copy.m_cdr)
{
}

inline
void
lcpp::SchemeCons::operator =(SchemeCons copy)
{
    std::swap(m_car, copy.m_car);
    std::swap(m_cdr, copy.m_cdr);
}

inline
lcpp::SchemeCons::~SchemeCons()
{
}

inline
lcpp::SchemeCons::SchemeCons(SchemeCons&& toMove) :
    m_car(toMove.m_car),
    m_cdr(toMove.m_cdr)
{
    toMove.m_car = toMove.m_cdr = &SCHEME_NIL;
}

inline
bool
lcpp::SchemeCons::operator ==(const SchemeObject& obj) const
{
    if (obj.is(SchemeType::Cons))
    {
        return static_cast<const SchemeCons&>(obj) == *this;
    }
    return false;
}

inline
bool
lcpp::SchemeCons::operator ==(const SchemeCons& rhs) const
{
   return car() == rhs.car()
       && cdr() == rhs.cdr();
}

inline
ezString
lcpp::SchemeCons::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("(%s %s)", m_car->toString().GetData(), m_cdr->toString().GetData());
    return builder.GetData();
}

// get car
inline
const lcpp::SchemeObject&
lcpp::SchemeCons::car() const
{
    EZ_ASSERT(m_car != nullptr, "Operating on invalid object!");
    return *m_car;
}

// set car
inline
void
lcpp::SchemeCons::car(const SchemeObject& value)
{
    EZ_ASSERT(&value != nullptr, "Invalid value for car!");
    m_car = &value;
}

// get cdr
inline
const lcpp::SchemeObject&
lcpp::SchemeCons::cdr() const
{
    EZ_ASSERT(m_cdr != nullptr, "Operating on invalid object!");
    return *m_cdr;
}

// set cdr
inline
void
lcpp::SchemeCons::cdr(const SchemeObject& value)
{
    EZ_ASSERT(&value != nullptr, "Invalid value for cdr!");
    m_cdr = &value;
}

