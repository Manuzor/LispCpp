#include "stdafx.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/recursionCounter.h"

#include "lcpp/core/exceptions/runtimeException.h"

#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/containers/stack.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::LispRuntimeState::LispRuntimeState() :
    m_stats(),
    m_recursionDepth(0),
    m_recursionLimit(255)
{
}

void
lcpp::LispRuntimeState::initialize()
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

    m_pSyntaxEnvironment = env::createTopLevel(symbol::create("syntax"));
    m_pGlobalEnvironment = env::create(symbol::create("global"), m_pSyntaxEnvironment);

    //////////////////////////////////////////////////////////////////////////
    
    m_pReaderState = LCPP_NEW(m_pAllocator.get(), reader::State)();
    m_pReaderState->m_pMacroEnv = env::createTopLevel(symbol::create("reader-macros"));

    // TODO reader, evaluator, printer setup.

    //////////////////////////////////////////////////////////////////////////

    registerBuiltIns();
}

void
lcpp::LispRuntimeState::shutdown()
{
    EZ_ASSERT(m_stats.m_initializationCount > 0,
              "LCPP_pRuntime was never initialized.");
    EZ_ASSERT(m_stats.m_initializationCount > m_stats.m_shutdownCount,
              "The LCPP_pRuntime was not shut down often enough!");

    ++m_stats.m_shutdownCount;

    LCPP_DELETE(m_pAllocator, m_pReaderState);
}

void
lcpp::LispRuntimeState::setRecursionLimit(ezUInt32 newLimit)
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

void lcpp::LispRuntimeState::increaseRecursionDepth()
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

void lcpp::LispRuntimeState::decreaseRecursionDepth()
{
    EZ_ASSERT(m_recursionDepth > 0, "Cannot decrease the recursion depth below 0!");
    --m_recursionDepth;
}
