
inline
lcpp::SchemeCons::SchemeCons(SchemeObject& car, SchemeObject& cdr) :
    m_pCar(&car),
    m_pCdr(&cdr)
{
}

inline
lcpp::SchemeCons::~SchemeCons()
{
}

inline
bool
lcpp::SchemeCons::operator ==(const SchemeObject& obj) const
{
    if (obj.is(TypeInfo<SchemeCons>::type()))
    {
        return static_cast<const SchemeCons&>(obj) == *this;
    }
    return false;
}

inline
bool
lcpp::SchemeCons::operator ==(const SchemeCons& rhs) const
{
    return *m_pCar == *rhs.m_pCar
        && *m_pCdr == *rhs.m_pCdr;
}

inline
ezString
lcpp::SchemeCons::toString() const
{
    ezStringBuilder builder;
    builder.Append('(');
    toStringHelper(builder);
    builder.Append(')');
    return builder;
}

inline
void
lcpp::SchemeCons::toStringHelper(ezStringBuilder& builder) const
{
    // car
    // \remark This is more efficient than simply calling m_car->toString()
    if (m_pCar->is(TypeInfo<SchemeCons>::type()))
    {
        builder.Append('(');
        static_cast<const SchemeCons*>(m_pCar)->toStringHelper(builder);
        builder.Append(')');
    }
    else
    {
        builder.Append(m_pCar->toString().GetData());
    }

    // cdr
    if(isNil(m_pCdr))
    {
        return;
    }

    builder.Append(' ');

    if (m_pCdr->is(TypeInfo<SchemeCons>::type()))
    {
        static_cast<const SchemeCons*>(m_pCdr)->toStringHelper(builder);
    }
    else
    {
        builder.AppendFormat(". %s", m_pCdr->toString().GetData());
    }
}

// set car
inline
void
lcpp::SchemeCons::car(lcpp::SchemeObject& object)
{
    m_pCar = &object;
}

// get car
inline
lcpp::SchemeObject&
lcpp::SchemeCons::car()
{
    return *m_pCar;
}

// set cdr
inline
void
lcpp::SchemeCons::cdr(lcpp::SchemeObject& object)
{
    m_pCdr = &object;
}

// get cdr
inline
lcpp::SchemeObject&
lcpp::SchemeCons::cdr()
{
    return *m_pCdr;
}
