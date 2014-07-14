#include "stdafx.h"
#include "lcpp/core/environment.h"

#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/runtime.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::Ptr<lcpp::Environment>
lcpp::Environment::createTopLevel(const ezString& name)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), Environment)(name);
}

lcpp::Ptr<lcpp::Environment>
lcpp::Environment::create(const ezString& name, Ptr<Environment> pParent)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), Environment)(name, pParent);
}

lcpp::Ptr<lcpp::Environment>
lcpp::Environment::create(Ptr<Environment> pParent)
{
    return create("", pParent);
}

lcpp::Environment::Environment(const ezString& name) :
    m_pParent(),
    m_name(LispRuntime::instance()->allocator().get()),
    m_symbols(LispRuntime::instance()->allocator().get())
{
    m_name = name;
}

lcpp::Environment::Environment(const ezString& name, Ptr<Environment> pParent) :
    m_pParent(pParent),
    m_name(LispRuntime::instance()->allocator().get()),
    m_symbols(LispRuntime::instance()->allocator().get())
{
    m_name = name;
}

void
lcpp::Environment::add(Ptr<LispSymbol> pKey, Ptr<LispObject> pValue)
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
lcpp::Environment::set(Ptr<LispSymbol> pKey, Ptr<LispObject> pValue)
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
lcpp::Environment::get(Ptr<LispSymbol> pKey, Ptr<LispObject>& out_value)
{
    EZ_LOG_BLOCK("Environment::get", pKey->value().GetData());
    ezLog::VerboseDebugMessage("Environment Name: %s", qualifiedName().GetData());

    LispObject* pResult = nullptr;
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
lcpp::Environment::exists(Ptr<LispSymbol> pKey)
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
