#include "lcpp/exceptions/exceptions.h"

inline
lcpp::Environment
lcpp::Environment::createTopLevelInstance()
{
    return Environment();
}

inline
lcpp::Environment::Environment(const ezString& name, Ptr<Environment> pParent) :
    m_pParent(pParent),
    m_name(name),
    m_symbols()
{
}

inline
lcpp::Environment::Environment() :
    m_pParent(nullptr),
    m_name(""),
    m_symbols()
{
}

inline
void
lcpp::Environment::set(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject> pValue)
{
    m_symbols[pKey.get()] = pValue.get();
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
    return m_symbols.KeyExists(pKey.get());
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


