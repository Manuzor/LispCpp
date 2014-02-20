
inline
lcpp::SchemeCons::SchemeCons(const SchemeObject& car, const SchemeObject& cdr) :
    m_car(&car),
    m_cdr(&cdr)
{
    EZ_ASSERT(m_car != this, "Cannot include myself as car!");
    EZ_ASSERT(m_cdr != this, "Cannot include myself as cdr!");
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
