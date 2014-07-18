
inline
const lcpp::Type&
lcpp::LispFunction::type() const
{
    return LispFunction::typeInfo();
}

inline
lcpp::Ptr<lcpp::LispSymbol>
lcpp::LispFunction::name()
{
    return m_pName;
}

inline
lcpp::Ptr<const lcpp::LispSymbol>
lcpp::LispFunction::name() const
{
    return m_pName;
}

inline
lcpp::Ptr<lcpp::Environment>
lcpp::LispFunction::env()
{
    return m_pEnv;
}

inline
lcpp::Ptr<const lcpp::Environment>
lcpp::LispFunction::env() const
{
    return m_pEnv;
}
