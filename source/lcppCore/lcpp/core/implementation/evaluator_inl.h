
inline
lcpp::Ptr<lcpp::LispRuntime>
lcpp::RecursiveEvaluator::runtime()
{
    return m_pRuntime;
}

inline
lcpp::Ptr<const lcpp::LispRuntime>
lcpp::RecursiveEvaluator::runtime() const
{
    return m_pRuntime;
}
