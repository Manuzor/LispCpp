
inline
lcpp::Ptr<lcpp::SchemeRuntime>
lcpp::RecursiveEvaluator::runtime()
{
    return m_pRuntime;
}

inline
lcpp::Ptr<const lcpp::SchemeRuntime>
lcpp::RecursiveEvaluator::runtime() const
{
    return m_pRuntime;
}
