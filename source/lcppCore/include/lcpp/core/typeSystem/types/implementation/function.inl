
inline
lcpp::SchemeFunction::SchemeFunction(Ptr<Environment> pEnv, Ptr<SchemeCons> pBody) :
    m_pEnv(pEnv),
    m_pBody(pBody)
{
}

inline
bool
lcpp::SchemeFunction::operator==(const SchemeObject& obj) const
{
    if (obj.is<SchemeFunction>())
    {
        return *this == static_cast<const SchemeFunction&>(obj);
    }
    return false;
}

inline
bool
lcpp::SchemeFunction::operator==(const SchemeFunction& rhs) const
{
    return &rhs == this;
}

inline
ezString
lcpp::SchemeFunction::toString() const
{
    // TODO implement this properly!
    return m_pBody->toString();
}
