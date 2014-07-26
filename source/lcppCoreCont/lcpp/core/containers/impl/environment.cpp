#include "stdafx.h"
#include "lcpp/core/containers/environment.h"

#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/nil.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

namespace lcpp
{

    Ptr<LispObject>
    LispEnvironment::createTopLevel(Ptr<LispObject> pName)
    {
        LCPP_NOT_IMPLEMENTED;
    }

    Ptr<LispObject>
    LispEnvironment::create(Ptr<LispObject> pName, Ptr<LispObject> pParent)
    {
        LCPP_NOT_IMPLEMENTED;
    }

    Ptr<LispObject>
    LispEnvironment::create(Ptr<LispObject> pParent)
    {
        LCPP_NOT_IMPLEMENTED;
    }

    Ptr<LispObject>
    LispEnvironment::create(const String& name, Ptr<LispObject> pParent)
    {
        LCPP_NOT_IMPLEMENTED;
    }


    Ptr<LispObject>
    LispEnvironment::createCopy(const LispEnvironment& toCopy)
    {
        LCPP_NOT_IMPLEMENTED;
    }

    LispEnvironment::LispEnvironment(Ptr<LispObject> pName) :
        m_pParent(),
        m_pName(pName),
        m_symbols(LispRuntime::instance()->allocator().get())
    {
    }

    LispEnvironment::LispEnvironment(Ptr<LispObject> pName, Ptr<LispObject> pParent) :
        m_pParent(pParent),
        m_pName(pName),
        m_symbols(LispRuntime::instance()->allocator().get())
    {
    }

    void
    LispEnvironment::add(Ptr<LispObject> pKey, Ptr<LispObject> pValue)
    {
        m_symbols[pKey.get()] = pValue.get();
    }

    ezResult
    LispEnvironment::set(Ptr<LispObject> pKey, Ptr<LispObject> pValue)
    {
        LCPP_NOT_IMPLEMENTED;

        //if(m_symbols.KeyExists(pKey.get()))
        //{
        //    m_symbols[pKey.get()] = pValue.get();
        //    return EZ_SUCCESS;
        //}
        //
        //if(m_pParent)
        //{
        //    return m_pParent->set(pKey, pValue);
        //}
        //
        //return EZ_FAILURE;
    }

    ezResult
    LispEnvironment::get(Ptr<LispObject> pKey, Ptr<LispObject>& out_value)
    {
        LCPP_NOT_IMPLEMENTED;

        //LispObject* pResult = nullptr;
        //if(m_symbols.TryGetValue(pKey.get(), pResult))
        //{
        //    out_value = pResult;
        //    return EZ_SUCCESS;
        //}
        //
        //if(m_pParent)
        //{
        //    return m_pParent->get(pKey, out_value);
        //}
        //
        //return EZ_FAILURE;
    }

    bool
    LispEnvironment::exists(Ptr<LispObject> pKey)
    {
        LCPP_NOT_IMPLEMENTED;

        //EZ_LOG_BLOCK("LispEnvironment::exists", pKey->value().GetData());
        //ezLog::VerboseDebugMessage("Name: %s", qualifiedName().GetData());
        //
        //auto existsLocally = m_symbols.KeyExists(pKey.get());
        //
        //if (existsLocally)
        //{
        //    ezLog::VerboseDebugMessage("Found. Value: %s", m_symbols[pKey.get()]->toString().GetData());
        //    return true;
        //}
        //
        //return m_pParent && m_pParent->exists(pKey);
    }

    void
    LispEnvironment::qualifiedNameHelper(ezStringBuilder& builder) const
    {
        LCPP_NOT_IMPLEMENTED;

        //if(!m_pParent) { return; }
        //
        //m_pParent->qualifiedNameHelper(builder);
        //builder.Append('/');
        //builder.Append(m_pName->value().GetData());
    }
}
