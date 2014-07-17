#include "stdafx.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/evaluator.h"
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
    
    m_pReader = Reader::create(Reader::CInfo());
    m_pEvaluator = RecursiveEvaluator::create();

    m_pSyntaxEnvironment = Environment::createTopLevel("syntax");
    m_pGlobalEnvironment = Environment::create("global", m_pSyntaxEnvironment);

    m_instanceTables.pSymbols = InstanceTable_Symbols::create();
    m_instanceTables.pIntegers = InstanceTable_Integers::create();

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
              LispFunctionBuiltin::create(name, pEnv, funcPtr))

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
    LCPP_ADD_BUILTIN_FUNCTION("exit", &builtIn::exit);
    LCPP_ADD_BUILTIN_FUNCTION("dump", &builtIn::dump);

    // REPL
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("read", &builtIn::read);
    LCPP_ADD_BUILTIN_FUNCTION("eval", &builtIn::eval);
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

    // Other
    //////////////////////////////////////////////////////////////////////////
    LCPP_ADD_BUILTIN_FUNCTION("set-recursion-limit", &builtIn::setRecursionLimit);
    LCPP_ADD_BUILTIN_FUNCTION("get-recursion-limit", &builtIn::getRecursionLimit);

    LCPP_ADD_BUILTIN_FUNCTION("cons", &builtIn::cons);
    LCPP_ADD_BUILTIN_FUNCTION("car", &builtIn::car);
    LCPP_ADD_BUILTIN_FUNCTION("cdr", &builtIn::cdr);

#undef LCPP_ADD_BUILTIN_FUNCTION
#undef LCPP_ADD_GLOBAL
}
