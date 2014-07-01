
inline
lcpp::Environment
lcpp::Environment::createTopLevelInstance()
{
    return Environment("", defaultAllocator());
}

inline
lcpp::Environment::Environment(const ezString& name, Ptr<ezAllocatorBase> pAllocator) :
    m_pAllocator(pAllocator),
    m_pParent(nullptr),
    m_name(m_pAllocator.get()),
    m_symbols(m_pAllocator.get())
{
    m_name = name;
}

inline
lcpp::Environment::Environment(const ezString& name, Ptr<Environment> pParent) :
    m_pAllocator(pParent->m_pAllocator),
    m_pParent(pParent),
    m_name(m_pAllocator.get()),
    m_symbols(m_pAllocator.get())
{
    m_name = name;
}

inline
lcpp::Ptr<lcpp::Environment>
lcpp::Environment::parent()
{
    return m_pParent;
}

inline
lcpp::Ptr<const lcpp::Environment>
lcpp::Environment::parent() const
{
    return m_pParent;
}

inline
const ezString&
lcpp::Environment::name() const
{
    return m_name;
}

inline
ezString&
lcpp::Environment::name()
{
    return m_name;
}

inline
ezString
lcpp::Environment::qualifiedName() const
{
    if(!m_pParent) { return "/"; }

    ezStringBuilder builder;
    qualifiedNameHelper(builder);
    return builder;
}

inline
void
lcpp::Environment::qualifiedNameHelper(ezStringBuilder& builder) const
{
    if (!m_pParent) { return; }

    m_pParent->qualifiedNameHelper(builder);
    builder.Append('/');
    builder.Append(m_name.GetData());
}


