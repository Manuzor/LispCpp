#include "lcpp/exceptions/exceptions.h"

inline
lcpp::Environment
lcpp::Environment::createTopLevelInstance()
{
    return Environment();
}

inline
lcpp::Environment::Environment(const ezString& name, Environment* pParent) :
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
lcpp::Environment::set(SchemeSymbol& key, SchemeObject& value)
{
    m_symbols[&key] = &value;
}

inline
ezResult
lcpp::Environment::get(SchemeSymbol& key, SchemeObject*& out_value)
{
    if(m_symbols.TryGetValue(&key, out_value))
    {
        return EZ_SUCCESS;
    }
    if (m_pParent)
    {
        return m_pParent->get(key, out_value);
    }
    
    return EZ_FAILURE;
}

inline
bool
lcpp::Environment::exists(SchemeSymbol& key)
{
    return m_symbols.KeyExists(&key);
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
    ezStringBuilder builder;
    qualifiedNameHelper(builder);
    return builder;
}

inline
void
lcpp::Environment::qualifiedNameHelper(ezStringBuilder& builder) const
{
    if (m_pParent)
    {
        builder.Append(m_name.GetData());
    }
    builder.AppendFormat("/%s", m_name.GetData());
}


