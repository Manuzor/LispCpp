#include "stdafx.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/recursionCounter.h"

#include "lcpp/core/exceptions/runtimeException.h"

#include "lcpp/core/containers/environment.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/containers/stack.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::Ptr<lcpp::LispRuntime> lcpp::LispRuntime::s_pInstance;

EZ_ON_GLOBAL_EVENT_ONCE(LCPP_GLOBAL_EVENT_STARTUP)
{
    lcpp::LispRuntime::instance()->initialize();
}

EZ_ON_GLOBAL_EVENT_ONCE(LCPP_GLOBAL_EVENT_SHUTDOWN)
{
    lcpp::LispRuntime::instance()->shutdown();
}

lcpp::Ptr<lcpp::LispRuntime>
lcpp::LispRuntime::defaultInstance()
{
    static auto pDefaultInstance = Ptr<LispRuntime>();

    if (!pDefaultInstance)
    {
        // TODO lock with mutex here.
        static auto defaultInstance = LispRuntime();
        pDefaultInstance = &defaultInstance;
    }
    

    return pDefaultInstance;
}

lcpp::Ptr<lcpp::LispRuntime>
lcpp::LispRuntime::instance()
{
    if (!s_pInstance)
    {
        patchInstance(defaultInstance());
    }
    
    return s_pInstance;
}

void
lcpp::LispRuntime::patchInstance(Ptr<LispRuntime> pNewInstance)
{
    // TODO: Look with mutex here
    s_pInstance = pNewInstance;
}

lcpp::LispRuntime::LispRuntime() :
    m_recursionDepth(0),
    m_recursionLimit(255)
{
}

void
lcpp::LispRuntime::initialize()
{
    m_pAllocator = defaultAllocator();

    m_pDefaultStack = LCPP_NEW(m_pAllocator.get(), Stack)();
    m_pStack = m_pDefaultStack;
    
    // TODO instanciate reader, evaluator, ...

    // TODO create instance tables

    //m_pSyntaxEnvironment = LispEnvironment::createTopLevel(LispSymbol::create("syntax"));
    //m_pGlobalEnvironment = LispEnvironment::create(LispSymbol::create("global"), m_pSyntaxEnvironment);

    //////////////////////////////////////////////////////////////////////////
    
    // TODO reader, evaluator, printer setup.

    //////////////////////////////////////////////////////////////////////////

    registerBuiltIns();
}

void
lcpp::LispRuntime::shutdown()
{
    LCPP_DELETE(m_pAllocator.get(), m_pDefaultStack.get());

    LCPP_DELETE(m_pAllocator.get(), m_pGlobalEnvironment.get());
    LCPP_DELETE(m_pAllocator.get(), m_pSyntaxEnvironment.get());
}

void
lcpp::LispRuntime::recursionLimit(ezUInt32 newLimit)
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

void lcpp::LispRuntime::increaseRecursionDepth()
{
    EZ_ASSERT(m_recursionDepth < m_recursionLimit, "Invalid current callStackDepth!");
    ++m_recursionDepth;
    if(m_recursionDepth >= m_recursionLimit)
    {
        ezStringBuilder message;
        message.Format("Exceeded max call stack depth of %u", m_recursionLimit);
        throw exceptions::Runtime(message.GetData());
    }
}

void lcpp::LispRuntime::decreaseRecursionDepth()
{
    EZ_ASSERT(m_recursionDepth > 0, "Cannot decrease the recursion depth below 0!");
    --m_recursionDepth;
}

void lcpp::LispRuntime::registerBuiltIns()
{
    // TODO implement me.
}
