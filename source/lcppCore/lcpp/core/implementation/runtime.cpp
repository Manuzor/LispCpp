#include "stdafx.h"

#include "lcpp/exceptions/exceptions.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/builtIn/recursive_functions.h"
#include "lcpp/core/recursionCounter.h"

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
    
    m_pReader = Reader::create(Reader::CInfo());
    m_pEvaluator = RecursiveEvaluator::create();

    m_instanceTables.pSymbols = InstanceTable_Symbols::create();
    m_instanceTables.pIntegers = InstanceTable_Integers::create();

    m_pSyntaxEnvironment = LispEnvironment::createTopLevel(LispSymbol::create("syntax"));
    m_pGlobalEnvironment = LispEnvironment::create(LispSymbol::create("global"), m_pSyntaxEnvironment);

    //////////////////////////////////////////////////////////////////////////
    
    m_pReader->initialize();
    m_pEvaluator->initialize();

    //////////////////////////////////////////////////////////////////////////

    registerBuiltIns();
}

void
lcpp::LispRuntime::shutdown()
{
    LCPP_DELETE(m_pAllocator, m_instanceTables.pIntegers);
    LCPP_DELETE(m_pAllocator, m_instanceTables.pSymbols);

    LCPP_DELETE(m_pAllocator, m_pGlobalEnvironment);
    LCPP_DELETE(m_pAllocator, m_pSyntaxEnvironment);

    LCPP_DELETE(m_pAllocator, m_pEvaluator);
    LCPP_DELETE(m_pAllocator, m_pReader);
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
#define LCPP_ADD_GLOBAL(nameString, value) pEnv->add(LispSymbol::create(nameString), value)
#define LCPP_ADD_BUILTIN_FUNCTION(nameString, functionPointer)              \
    pSymbol = LispSymbol::create(nameString);                               \
    pBuiltInFunction = LispFunction_BuiltIn::create(pEnv, functionPointer); \
    pBuiltInFunction->name(pSymbol);                                        \
    pEnv->add(pSymbol, pBuiltInFunction);

    auto pEnv = globalEnvironment();
    Ptr<LispFunction_BuiltIn> pBuiltInFunction;
    Ptr<LispSymbol> pSymbol;

    // Globals
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_GLOBAL("#t", LCPP_TRUE);
    LCPP_ADD_GLOBAL("#f", LCPP_FALSE);
    LCPP_ADD_GLOBAL("null", LCPP_NIL);
    LCPP_ADD_GLOBAL("nil", LCPP_NIL);
    LCPP_ADD_GLOBAL("#v", LCPP_VOID);

    // Utility functions
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("exit", &builtIn::exit);
    LCPP_ADD_BUILTIN_FUNCTION("dump", &builtIn::dump);

    // REPL
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("read", &builtIn::read);
    LCPP_ADD_BUILTIN_FUNCTION("eval", &builtIn::eval);
    LCPP_ADD_BUILTIN_FUNCTION("eval-in", &builtIn::evalIn);
    LCPP_ADD_BUILTIN_FUNCTION("print", &builtIn::print);

    // File handling
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("file-open", &builtIn::fileOpen);
    LCPP_ADD_BUILTIN_FUNCTION("file-is-open", &builtIn::fileIsOpen);
    LCPP_ADD_BUILTIN_FUNCTION("file-read-string", &builtIn::fileReadString);
    LCPP_ADD_BUILTIN_FUNCTION("file-close", &builtIn::fileClose);

    // Basic math
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("+", &builtIn::add);
    LCPP_ADD_BUILTIN_FUNCTION("-", &builtIn::sub);
    LCPP_ADD_BUILTIN_FUNCTION("*", &builtIn::mul);
    LCPP_ADD_BUILTIN_FUNCTION("=", &builtIn::equals);
    LCPP_ADD_BUILTIN_FUNCTION("modulo", &builtIn::modulo);

    LCPP_ADD_BUILTIN_FUNCTION("eq?", &builtIn::objectEquals);
    LCPP_ADD_BUILTIN_FUNCTION(">",  &builtIn::greaterThan);
    LCPP_ADD_BUILTIN_FUNCTION(">=", &builtIn::greaterOrEqual);
    LCPP_ADD_BUILTIN_FUNCTION("<",  &builtIn::lowerThan);
    LCPP_ADD_BUILTIN_FUNCTION("<=", &builtIn::lowerOrEqual);

    // Other
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("set-recursion-limit", &builtIn::setRecursionLimit);
    LCPP_ADD_BUILTIN_FUNCTION("get-recursion-limit", &builtIn::getRecursionLimit);

    LCPP_ADD_BUILTIN_FUNCTION("cons", &builtIn::cons);
    LCPP_ADD_BUILTIN_FUNCTION("car", &builtIn::car);
    LCPP_ADD_BUILTIN_FUNCTION("cdr", &builtIn::cdr);

    LCPP_ADD_BUILTIN_FUNCTION("format", &builtIn::format);

    LCPP_ADD_BUILTIN_FUNCTION("env-get-global", &builtIn::envGetGlobal);


#undef LCPP_ADD_BUILTIN_FUNCTION
#undef LCPP_ADD_GLOBAL
}
