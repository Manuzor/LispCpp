#include "stdafx.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/typeFactory.h"
#include "lcpp/core/builtIn/recursive_functions.h"

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

    //////////////////////////////////////////////////////////////////////////

    registerBuiltInFunctions();
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

void lcpp::SchemeRuntime::registerBuiltInFunctions()
{
    auto pEnv = globalEnvironment();
    // constants
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(factory()->createSymbol("#t"), SCHEME_TRUE_PTR);
    pEnv->add(factory()->createSymbol("#f"), SCHEME_FALSE_PTR);
    pEnv->add(factory()->createSymbol("null"), SCHEME_NIL_PTR);
    pEnv->add(factory()->createSymbol("nil"), SCHEME_NIL_PTR);
    pEnv->add(factory()->createSymbol("#v"), SCHEME_VOID_PTR);

    // Utility functions
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(factory()->createSymbol("exit"),
              factory()->createBuiltinFunction("exit", pEnv, &builtin::exit));
    pEnv->add(factory()->createSymbol("dump"),
              factory()->createBuiltinFunction("dump", pEnv, &builtin::dump));

    // REPL
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(factory()->createSymbol("read"),
              factory()->createBuiltinFunction("read", pEnv, &builtin::read));
    pEnv->add(factory()->createSymbol("eval"),
              factory()->createBuiltinFunction("eval", pEnv, &builtin::eval));
    pEnv->add(factory()->createSymbol("print"),
              factory()->createBuiltinFunction("print", pEnv, &builtin::print));

    // File handling
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(factory()->createSymbol("file-open"),
              factory()->createBuiltinFunction("file-open", pEnv, &builtin::fileOpen));
    pEnv->add(factory()->createSymbol("file-is-open"),
              factory()->createBuiltinFunction("file-is-open", pEnv, &builtin::fileIsOpen));
    pEnv->add(factory()->createSymbol("file-read-string"),
              factory()->createBuiltinFunction("file-read-string", pEnv, &builtin::fileReadString));
    pEnv->add(factory()->createSymbol("file-close"),
              factory()->createBuiltinFunction("file-close", pEnv, &builtin::fileClose));

    // Basic math
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(factory()->createSymbol("+"),
              factory()->createBuiltinFunction("+", pEnv, &builtin::add));
    pEnv->add(factory()->createSymbol("-"),
              factory()->createBuiltinFunction("-", pEnv, &builtin::sub));
    pEnv->add(factory()->createSymbol("*"),
              factory()->createBuiltinFunction("*", pEnv, &builtin::mul));

    // Other
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(factory()->createSymbol("set-recursion-limit"),
              factory()->createBuiltinFunction("set-recursion-limit", pEnv, &builtin::setRecursionLimit));
    pEnv->add(factory()->createSymbol("get-recursion-limit"),
              factory()->createBuiltinFunction("get-recursion-limit", pEnv, &builtin::getRecursionLimit));
}
