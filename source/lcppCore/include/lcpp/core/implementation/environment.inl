#include "lcpp/exceptions/exceptions.h"

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
void
lcpp::Environment::add(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject> pValue)
{
    m_symbols[pKey.get()] = pValue.get();
}

inline
ezResult
lcpp::Environment::set(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject> pValue)
{
    if(m_symbols.KeyExists(pKey.get()))
    {
        m_symbols[pKey.get()] = pValue.get();
        return EZ_SUCCESS;
    }
    
    if(m_pParent)
    {
        return m_pParent->set(pKey, pValue);
    }

    return EZ_FAILURE;
}

inline
ezResult
lcpp::Environment::get(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject>& out_value)
{
    SchemeObject* pResult = nullptr;
    if(m_symbols.TryGetValue(pKey.get(), pResult))
    {
        out_value = pResult;
        return EZ_SUCCESS;
    }
    if (m_pParent)
    {
        return m_pParent->get(pKey, out_value);
    }
    
    return EZ_FAILURE;
}

inline
bool
lcpp::Environment::exists(Ptr<SchemeSymbol> pKey)
{
    return m_symbols.KeyExists(pKey.get())
        || m_pParent && m_pParent->exists(pKey);
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


