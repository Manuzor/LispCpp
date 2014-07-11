#include "stdafx.h"
#include "lcpp/core/environment.h"

#include "lcpp/core/typeSystem/types/symbol.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

void
lcpp::Environment::add(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject> pValue)
{
    EZ_LOG_BLOCK("Environment::add", pKey->value().GetData());
    ezLog::VerboseDebugMessage("Environment Name: %s", qualifiedName().GetData());

    if (m_symbols.KeyExists(pKey.get()))
    {
        EZ_LOG_BLOCK("Replacing existing value for key", pKey->value().GetData());
        ezLog::VerboseDebugMessage("Old value: %s", m_symbols[pKey.get()]->toString().GetData());
        ezLog::VerboseDebugMessage("New value: %s", pValue->toString().GetData());
    }
    else
    {
        ezLog::VerboseDebugMessage("Adding new value: %s", pValue->toString().GetData());
    }
    
    m_symbols[pKey.get()] = pValue.get();
}

ezResult
lcpp::Environment::set(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject> pValue)
{
    EZ_LOG_BLOCK("Environment::set", pKey->value().GetData());
    ezLog::VerboseDebugMessage("Environment Name: %s", qualifiedName().GetData());

    if(m_symbols.KeyExists(pKey.get()))
    {
        m_symbols[pKey.get()] = pValue.get();
        ezLog::VerboseDebugMessage("Key found.");
        return EZ_SUCCESS;
    }

    if(m_pParent)
    {
        return m_pParent->set(pKey, pValue);
    }

    return EZ_FAILURE;
}

ezResult
lcpp::Environment::get(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject>& out_value)
{
    EZ_LOG_BLOCK("Environment::get", pKey->value().GetData());
    ezLog::VerboseDebugMessage("Environment Name: %s", qualifiedName().GetData());

    SchemeObject* pResult = nullptr;
    if(m_symbols.TryGetValue(pKey.get(), pResult))
    {
        out_value = pResult;
        ezLog::VerboseDebugMessage("Found value: %s", pResult->toString().GetData());
        return EZ_SUCCESS;
    }
    if(m_pParent)
    {
        return m_pParent->get(pKey, out_value);
    }

    return EZ_FAILURE;
}

bool
lcpp::Environment::exists(Ptr<SchemeSymbol> pKey)
{
    EZ_LOG_BLOCK("Environment::exists", pKey->value().GetData());
    ezLog::VerboseDebugMessage("Name: %s", qualifiedName().GetData());

    auto existsLocally = m_symbols.KeyExists(pKey.get());

    if (existsLocally)
    {
        ezLog::VerboseDebugMessage("Found. Value: %s", m_symbols[pKey.get()]->toString().GetData());
    }

    return existsLocally
        || m_pParent && m_pParent->exists(pKey);
}
