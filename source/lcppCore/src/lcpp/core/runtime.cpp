#include "stdafx.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/typeFactory.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::SchemeRuntime::SchemeRuntime() :
    m_recursionDepth(0),
    m_recursionLimit(255)
{
}

void
lcpp::SchemeRuntime::initialize()
{
    m_pAllocator = defaultAllocator();
    m_pFactory = LCPP_NEW(m_pAllocator, TypeFactory)(this);
    
    m_pReader = LCPP_NEW(m_pAllocator, Reader)(this, Reader::CInfo());
    m_pEvaluator = LCPP_NEW(m_pAllocator, RecursiveEvaluator)(this);

    m_pSyntaxEnvironment = LCPP_NEW(m_pAllocator, Environment)("syntax", m_pAllocator);
    m_pGlobalEnvironment = LCPP_NEW(m_pAllocator, Environment)("global", m_pSyntaxEnvironment);

    //////////////////////////////////////////////////////////////////////////
    
    m_pReader->initialize();
    m_pEvaluator->initialize();
}

void
lcpp::SchemeRuntime::shutdown()
{
    LCPP_DELETE(m_pAllocator.get(), m_pGlobalEnvironment.get());
    LCPP_DELETE(m_pAllocator.get(), m_pSyntaxEnvironment.get());

    LCPP_DELETE(m_pAllocator.get(), m_pEvaluator.get());
    LCPP_DELETE(m_pAllocator.get(), m_pReader.get());

    LCPP_DELETE(m_pAllocator.get(), m_pFactory.get());
}

lcpp::SchemeRuntime::RecursionCounter
lcpp::SchemeRuntime::createRecursionCounter()
{
    EZ_ASSERT(m_recursionDepth < m_recursionLimit, "Invalid current callStackDepth!");
    if(m_recursionDepth + 1 >= m_recursionLimit)
    {
        ezStringBuilder message;
        message.Format("Exceeded max call stack depth of %u", m_recursionLimit);
        throw exceptions::Runtime(message.GetData());
    }
    
    return RecursionCounter(m_recursionDepth);
}

void
lcpp::SchemeRuntime::recursionLimit(ezUInt32 newLimit)
{
    if(m_recursionDepth >= newLimit)
    {
        ezStringBuilder message;
        message.Format("Cannot set new recursion limit from %u to %u "
                       "because the current recursion depth is at %u.",
                       m_recursionLimit, newLimit, m_recursionDepth);
        throw exceptions::Runtime(message.GetData());
    }
    m_recursionLimit = newLimit;
}
