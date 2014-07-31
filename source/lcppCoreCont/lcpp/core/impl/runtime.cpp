#include "stdafx.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/recursionCounter.h"

#include "lcpp/core/exceptions/runtimeException.h"

#include "lcpp/core/typeSystem/object.h"
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
    m_stats(),
    m_recursionDepth(0),
    m_recursionLimit(255)
{
}

void
lcpp::LispRuntime::initialize()
{
    EZ_ASSERT(m_stats.m_shutdownCount <= m_stats.m_initializationCount,
              "LCPP_pRuntime was shut down more often than it was initialized!");

    if (m_stats.m_shutdownCount < m_stats.m_initializationCount)
    {
        shutdown();
    }

    EZ_ASSERT(m_stats.m_initializationCount == m_stats.m_shutdownCount,
              "LCPP_pRuntime initialization and shutdown count must be balanced!");

    ++m_stats.m_initializationCount;

    m_pAllocator = defaultAllocator();
    
    // TODO instanciate reader, evaluator, ...

    // TODO create instance tables
    m_instanceTables.m_pSymbolTable = LCPP_NEW(m_pAllocator.get(), InsanceTable<symbol::String>)(&symbol::createNew);

    //m_pSyntaxEnvironment = LispEnvironment::createTopLevel(LispSymbol::create("syntax"));
    //m_pGlobalEnvironment = LispEnvironment::create(LispSymbol::create("global"), m_pSyntaxEnvironment);

    //////////////////////////////////////////////////////////////////////////
    
    m_pReaderState = LCPP_NEW(m_pAllocator.get(), reader::State)();

    // TODO reader, evaluator, printer setup.

    //////////////////////////////////////////////////////////////////////////

    registerBuiltIns();
}

void
lcpp::LispRuntime::shutdown()
{
    EZ_ASSERT(m_stats.m_initializationCount > 0,
              "LCPP_pRuntime was never initialized.");
    EZ_ASSERT(m_stats.m_initializationCount > m_stats.m_shutdownCount,
              "The LCPP_pRuntime was not shut down often enough!");

    ++m_stats.m_shutdownCount;

    LCPP_DELETE(m_pAllocator.get(), m_pReaderState.get());

    LCPP_DELETE(m_pAllocator.get(), m_instanceTables.m_pSymbolTable.get());

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
