#include "stdafx.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/recursionCounter.h"

#include "lcpp/core/exceptions/runtimeException.h"

#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/containers/stack.h"

#ifndef VerboseDebugMessage
// Enable this to allow verbose debug messages
#define VerboseDebugMessage Debug
#endif

#if EZ_ENABLED(EZ_COMPILE_FOR_DEBUG)
lcpp::GarbageCollector* g_pGC(nullptr);
#endif

lcpp::LispRuntimeState::LispRuntimeState() :
    m_stats(),
    m_recursionDepth(0),
    m_recursionLimit(255),
    m_baseDirectory("."),
    m_userDirectory(".")
{
}

void
lcpp::LispRuntimeState::initialize(ezAllocatorBase* pAllocator)
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

    m_pAllocator = pAllocator ? pAllocator : defaultAllocator();
    m_pGC = lcpp::getGarbageCollector();

#if EZ_ENABLED(EZ_COMPILE_FOR_DEBUG)
    g_pGC = m_pGC.get();
#endif

    m_pSyntaxEnvironment = env::createTopLevel(symbol::create("syntax")).get();
    m_pGC->addRoot(m_pSyntaxEnvironment);
    m_pGlobalEnvironment = env::create(getSyntaxEnvironment(), symbol::create("global")).get();
    m_pGC->addRoot(m_pGlobalEnvironment);

    //////////////////////////////////////////////////////////////////////////

    m_readerState.reset();
    m_readerState.m_pMacroEnv = env::createTopLevel(symbol::create("reader-macros"));
    m_pGC->addRoot(m_readerState.m_pMacroEnv.get());

    m_printerState.reset();

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

    m_pGC->removeRoot(m_readerState.m_pMacroEnv.get());
    m_pGC->removeRoot(m_pGlobalEnvironment);
    m_pGC->removeRoot(m_pSyntaxEnvironment);

    m_readerState.m_pMacroEnv = nullptr;
    m_printerState.m_pOutStream = &m_printerState.m_stdOutStream;
    m_pGlobalEnvironment = nullptr;
    m_pSyntaxEnvironment = nullptr;

    // TODO Once we have a per-runtime garbage collector system running, uncomment the following line.
    //m_pGC->clear();
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
        // Reset the recursion depth.
        m_recursionDepth = 0;

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
