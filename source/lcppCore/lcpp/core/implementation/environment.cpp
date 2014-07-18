#include "stdafx.h"
#include "lcpp/core/environment.h"

#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/nil.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::Ptr<lcpp::LispEnvironment>
lcpp::LispEnvironment::createTopLevel(Ptr<LispSymbol> pName)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), LispEnvironment)(pName ? pName : LispSymbol::create(""));
}

lcpp::Ptr<lcpp::LispEnvironment>
lcpp::LispEnvironment::create(Ptr<LispSymbol> pName, Ptr<LispEnvironment> pParent)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), LispEnvironment)(pName, pParent);
}

lcpp::Ptr<lcpp::LispEnvironment>
lcpp::LispEnvironment::create(Ptr<LispEnvironment> pParent)
{
    return create(LispSymbol::create("anonymous"), pParent);
}

lcpp::Ptr<lcpp::LispEnvironment>
lcpp::LispEnvironment::create(const ezString& name, Ptr<LispEnvironment> pParent)
{
    return create(LispSymbol::create(name), pParent);
}


lcpp::Ptr<lcpp::LispEnvironment>
lcpp::LispEnvironment::createCopy(const LispEnvironment& toCopy)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), LispEnvironment)(toCopy);
}

const lcpp::Type&
lcpp::LispEnvironment::typeInfo()
{
    static auto t = Type::create(Type::Flags::None,
                                 EZ_STRINGIZE(LispEnvironment),
                                 "environment",
                                 MemoryInfo(sizeof(LispEnvironment),
                                            EZ_ALIGNMENT_OF(LispEnvironment)));

    return t;
}

lcpp::LispEnvironment::LispEnvironment(Ptr<LispSymbol> pName) :
    m_pParent(),
    m_pName(pName),
    m_symbols(LispRuntime::instance()->allocator().get())
{
}

lcpp::LispEnvironment::LispEnvironment(Ptr<LispSymbol> pName, Ptr<LispEnvironment> pParent) :
    m_pParent(pParent),
    m_pName(pName),
    m_symbols(LispRuntime::instance()->allocator().get())
{
}

void
lcpp::LispEnvironment::add(Ptr<LispSymbol> pKey, Ptr<LispObject> pValue)
{
    EZ_LOG_BLOCK("LispEnvironment::add", pKey->value().GetData());
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
lcpp::LispEnvironment::set(Ptr<LispSymbol> pKey, Ptr<LispObject> pValue)
{
    EZ_LOG_BLOCK("LispEnvironment::set", pKey->value().GetData());
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
lcpp::LispEnvironment::get(Ptr<LispSymbol> pKey, Ptr<LispObject>& out_value)
{
    EZ_LOG_BLOCK("LispEnvironment::get", pKey->value().GetData());
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
lcpp::LispEnvironment::exists(Ptr<LispSymbol> pKey)
{
    EZ_LOG_BLOCK("LispEnvironment::exists", pKey->value().GetData());
    ezLog::VerboseDebugMessage("Name: %s", qualifiedName().GetData());

    auto existsLocally = m_symbols.KeyExists(pKey.get());

    if (existsLocally)
    {
        ezLog::VerboseDebugMessage("Found. Value: %s", m_symbols[pKey.get()]->toString().GetData());
    }

    return existsLocally
        || m_pParent && m_pParent->exists(pKey);
}

void
lcpp::LispEnvironment::qualifiedNameHelper(ezStringBuilder& builder) const
{
    if(!m_pParent) { return; }

    m_pParent->qualifiedNameHelper(builder);
    builder.Append('/');
    builder.Append(m_pName->value().GetData());
}
