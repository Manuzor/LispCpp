#include "stdafx.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/typeFactory.h"
#include "lcpp/core/builtIn/recursive_functions.h"
#include "lcpp/core/recursionCounter.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::Ptr<lcpp::LispRuntime> lcpp::LispRuntime::s_pInstance;

lcpp::Ptr<lcpp::LispRuntime>
lcpp::LispRuntime::defaultInstance()
{
    static auto defaultInstance = LispRuntime();
    static auto pDefaultInstance = Ptr<LispRuntime>(&defaultInstance);
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
    if (!s_pInstance)
    {
        s_pInstance = pNewInstance;
    }
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
    m_pFactory = LCPP_NEW(m_pAllocator, TypeFactory)();
    
    m_pReader = LCPP_NEW(m_pAllocator, Reader)(Reader::CInfo());
    m_pEvaluator = LCPP_NEW(m_pAllocator, RecursiveEvaluator)();

    m_pSyntaxEnvironment = Environment::createTopLevel("syntax");
    m_pGlobalEnvironment = Environment::create("global", m_pSyntaxEnvironment);

    m_instanceTables.pSymbols = LCPP_NEW(m_pAllocator, InstanceTable_Symbols)(this);
    m_instanceTables.pIntegers = LCPP_NEW(m_pAllocator, InstanceTable_Integers)(this);

    //////////////////////////////////////////////////////////////////////////
    
    m_pReader->initialize();
    m_pEvaluator->initialize();

    //////////////////////////////////////////////////////////////////////////

    registerBuiltIns();
}

void
lcpp::LispRuntime::shutdown()
{
    LCPP_DELETE(m_pAllocator.get(), m_instanceTables.pIntegers.get());
    LCPP_DELETE(m_pAllocator.get(), m_instanceTables.pSymbols.get());

    LCPP_DELETE(m_pAllocator.get(), m_pGlobalEnvironment.get());
    LCPP_DELETE(m_pAllocator.get(), m_pSyntaxEnvironment.get());

    LCPP_DELETE(m_pAllocator.get(), m_pEvaluator.get());
    LCPP_DELETE(m_pAllocator.get(), m_pReader.get());

    LCPP_DELETE(m_pAllocator.get(), m_pFactory.get());
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
#define LCPP_ADD_GLOBAL(name, value) pEnv->add(LispSymbol::create(name), value)
#define LCPP_ADD_BUILTIN_FUNCTION(name, funcPtr) \
    pEnv->add(LispSymbol::create(name),     \
              factory()->createBuiltinFunction(name, pEnv, funcPtr))

    auto pEnv = globalEnvironment();

    // Globals
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_GLOBAL("#t", LCPP_TRUE);
    LCPP_ADD_GLOBAL("#f", LCPP_FALSE);
    LCPP_ADD_GLOBAL("null", LCPP_NIL);
    LCPP_ADD_GLOBAL("nil", LCPP_NIL);
    LCPP_ADD_GLOBAL("#v", LCPP_VOID);

    // Utility functions
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("exit", &builtin::exit);
    LCPP_ADD_BUILTIN_FUNCTION("dump", &builtin::dump);

    // REPL
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("read", &builtin::read);
    LCPP_ADD_BUILTIN_FUNCTION("eval", &builtin::eval);
    LCPP_ADD_BUILTIN_FUNCTION("print", &builtin::print);

    // File handling
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("file-open", &builtin::fileOpen);
    LCPP_ADD_BUILTIN_FUNCTION("file-is-open", &builtin::fileIsOpen);
    LCPP_ADD_BUILTIN_FUNCTION("file-read-string", &builtin::fileReadString);
    LCPP_ADD_BUILTIN_FUNCTION("file-close", &builtin::fileClose);

    // Basic math
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("+", &builtin::add);
    LCPP_ADD_BUILTIN_FUNCTION("-", &builtin::sub);
    LCPP_ADD_BUILTIN_FUNCTION("*", &builtin::mul);
    LCPP_ADD_BUILTIN_FUNCTION("=", &builtin::equals);
    LCPP_ADD_BUILTIN_FUNCTION("modulo", &builtin::modulo);

    LCPP_ADD_BUILTIN_FUNCTION("eq?", &builtin::objectEquals);

    // Other
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("set-recursion-limit", &builtin::setRecursionLimit);
    LCPP_ADD_BUILTIN_FUNCTION("get-recursion-limit", &builtin::getRecursionLimit);

    LCPP_ADD_BUILTIN_FUNCTION("cons", &builtin::cons);
    LCPP_ADD_BUILTIN_FUNCTION("car", &builtin::car);
    LCPP_ADD_BUILTIN_FUNCTION("cdr", &builtin::cdr);

#undef LCPP_ADD_BUILTIN_FUNCTION
#undef LCPP_ADD_GLOBAL
}
