
inline
lcpp::Ptr<ezAllocatorBase>
lcpp::LispRuntime::allocator()
{
    return m_pAllocator;
}

inline
void
lcpp::LispRuntime::allocator(lcpp::Ptr<ezAllocatorBase> value)
{
    m_pAllocator = value;
}

inline
lcpp::Ptr<const lcpp::Reader>
lcpp::LispRuntime::reader() const
{
    return m_pReader;
}

inline
lcpp::Ptr<lcpp::Reader>
lcpp::LispRuntime::reader()
{
    return m_pReader;
}

inline
void
lcpp::LispRuntime::reader(lcpp::Ptr<lcpp::Reader> value)
{
    m_pReader = value;
}

inline
lcpp::Ptr<const lcpp::IEvaluator>
lcpp::LispRuntime::evaluator() const
{
    return m_pEvaluator;
}

inline
lcpp::Ptr<lcpp::IEvaluator>
lcpp::LispRuntime::evaluator()
{
    return m_pEvaluator;
}

inline
void
lcpp::LispRuntime::evaluator(lcpp::Ptr<lcpp::IEvaluator> value)
{
    m_pEvaluator = value;
}

inline
lcpp::Ptr<const lcpp::LispEnvironment>
lcpp::LispRuntime::syntaxEnvironment() const
{
    return m_pSyntaxEnvironment;
}

inline
lcpp::Ptr<lcpp::LispEnvironment>
lcpp::LispRuntime::syntaxEnvironment()
{
    return m_pSyntaxEnvironment;
}

inline
void
lcpp::LispRuntime::syntaxEnvironment(lcpp::Ptr<lcpp::LispEnvironment> value)
{
    m_pSyntaxEnvironment = value;
}

inline
lcpp::Ptr<const lcpp::LispEnvironment>
lcpp::LispRuntime::globalEnvironment() const
{
    return m_pGlobalEnvironment;
}

inline
lcpp::Ptr<lcpp::LispEnvironment>
lcpp::LispRuntime::globalEnvironment()
{
    return m_pGlobalEnvironment;
}

inline
void
lcpp::LispRuntime::globalEnvironment(lcpp::Ptr<lcpp::LispEnvironment> value)
{
    m_pGlobalEnvironment = value;
}

inline
const lcpp::LispRuntime::InstanceTables&
lcpp::LispRuntime::instanceTables() const
{
    return m_instanceTables;
}

inline
ezUInt32
lcpp::LispRuntime::recursionDepth() const
{
    return m_recursionDepth;
}

inline
ezUInt32
lcpp::LispRuntime::recursionLimit() const
{
    return m_recursionLimit;
}
