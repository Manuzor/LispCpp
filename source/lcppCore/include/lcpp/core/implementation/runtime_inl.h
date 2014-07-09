
inline
lcpp::Ptr<ezAllocatorBase>
lcpp::SchemeRuntime::allocator()
{
    return m_pAllocator;
}

inline
void
lcpp::SchemeRuntime::allocator(lcpp::Ptr<ezAllocatorBase> value)
{
    m_pAllocator = value;
}

inline
lcpp::Ptr<const lcpp::TypeFactory>
lcpp::SchemeRuntime::factory() const
{
    return m_pFactory;
}

inline
lcpp::Ptr<lcpp::TypeFactory>
lcpp::SchemeRuntime::factory()
{
    return m_pFactory;
}

inline
void
lcpp::SchemeRuntime::factory(lcpp::Ptr<lcpp::TypeFactory> value)
{
    m_pFactory = value;
}

inline
lcpp::Ptr<const lcpp::Reader>
lcpp::SchemeRuntime::reader() const
{
    return m_pReader;
}

inline
lcpp::Ptr<lcpp::Reader>
lcpp::SchemeRuntime::reader()
{
    return m_pReader;
}

inline
void
lcpp::SchemeRuntime::reader(lcpp::Ptr<lcpp::Reader> value)
{
    m_pReader = value;
}

inline
lcpp::Ptr<const lcpp::IEvaluator>
lcpp::SchemeRuntime::evaluator() const
{
    return m_pEvaluator;
}

inline
lcpp::Ptr<lcpp::IEvaluator>
lcpp::SchemeRuntime::evaluator()
{
    return m_pEvaluator;
}

inline
void
lcpp::SchemeRuntime::evaluator(lcpp::Ptr<lcpp::IEvaluator> value)
{
    m_pEvaluator = value;
}

inline
lcpp::Ptr<const lcpp::Environment>
lcpp::SchemeRuntime::syntaxEnvironment() const
{
    return m_pSyntaxEnvironment;
}

inline
lcpp::Ptr<lcpp::Environment>
lcpp::SchemeRuntime::syntaxEnvironment()
{
    return m_pSyntaxEnvironment;
}

inline
void
lcpp::SchemeRuntime::syntaxEnvironment(lcpp::Ptr<lcpp::Environment> value)
{
    m_pSyntaxEnvironment = value;
}

inline
lcpp::Ptr<const lcpp::Environment>
lcpp::SchemeRuntime::globalEnvironment() const
{
    return m_pGlobalEnvironment;
}

inline
lcpp::Ptr<lcpp::Environment>
lcpp::SchemeRuntime::globalEnvironment()
{
    return m_pGlobalEnvironment;
}

inline
void
lcpp::SchemeRuntime::globalEnvironment(lcpp::Ptr<lcpp::Environment> value)
{
    m_pGlobalEnvironment = value;
}

inline
ezUInt32
lcpp::SchemeRuntime::recursionDepth() const
{
    return m_recursionDepth;
}

inline
ezUInt32
lcpp::SchemeRuntime::recursionLimit() const
{
    return m_recursionLimit;
}
