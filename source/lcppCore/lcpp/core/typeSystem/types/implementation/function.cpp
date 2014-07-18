#include "stdafx.h"

#include "lcpp/core/typeSystem.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/recursionCounter.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

const lcpp::Type&
lcpp::LispFunction::typeInfo()
{
    static auto t = Type::create(Type::Flags::Abstract | Type::Flags::Callable,
                                 EZ_STRINGIZE(LispFunction),
                                 "function (base)",
                                 MemoryInfo(sizeof(LispFunction),
                                            sizeof(LispFunction)));

    return t;
}

lcpp::LispFunction::LispFunction(Ptr<Environment> pEnv) :
    m_pName(),
    m_pEnv(pEnv)
{
}

bool
lcpp::LispFunction::operator==(const LispObject& obj) const
{
    if (obj.is<LispFunction>())
    {
        return *this == static_cast<const LispFunction&>(obj);
    }
    return false;
}

bool
lcpp::LispFunction::operator==(const LispFunction& rhs) const
{
    // Identity check.
    return &rhs == this;
}

void
lcpp::LispFunction::name(Ptr<LispSymbol> pName)
{
    EZ_ASSERT(pName, "The passed symbol instance is invalid!");
    m_pName = pName;
    m_pEnv->name(pName);
}

//////////////////////////////////////////////////////////////////////////

lcpp::Ptr<lcpp::LispFunction_BuiltIn>
lcpp::LispFunction_BuiltIn::create(Ptr<Environment> pParentEnv,
                                   ExecutorPtr_t executor)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    auto pNewEnv = Environment::create(pParentEnv);
    return LCPP_NEW(pAllocator, LispFunction_BuiltIn)(pNewEnv, executor);
}

lcpp::Ptr<lcpp::LispFunction_BuiltIn>
lcpp::LispFunction_BuiltIn::create(const ezString& name,
                                   Ptr<Environment> pParentEnv,
                                   ExecutorPtr_t executor)
{
    auto pFunction = create(pParentEnv, executor);
    pFunction->name(LispSymbol::create(name));
    return pFunction;
}


lcpp::Ptr<lcpp::LispFunction_BuiltIn>
lcpp::LispFunction_BuiltIn::copy(const LispFunction_BuiltIn& toCopy)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    return LCPP_NEW(pAllocator, LispFunction_BuiltIn)(toCopy);
}

const lcpp::Type&
lcpp::LispFunction_BuiltIn::typeInfo()
{
    static auto t = Type::create(Type::Flags::Callable,
                                 EZ_STRINGIZE(LispFunction_BuiltIn),
                                 "function (built-in)",
                                 MemoryInfo(sizeof(LispFunction_BuiltIn),
                                 sizeof(LispFunction_BuiltIn)));

    return t;
}

lcpp::LispFunction_BuiltIn::LispFunction_BuiltIn(Ptr<Environment> pEnv,
                                                 ExecutorPtr_t pExec) :
    LispFunction(pEnv),
    m_pExec(pExec)
{
    EZ_ASSERT(pExec, "The function executor must be valid!");
}

lcpp::Ptr<lcpp::LispObject>
lcpp::LispFunction_BuiltIn::copy() const
{
    return copy(*this);
}

ezString
lcpp::LispFunction_BuiltIn::toString() const
{
    EZ_ASSERT(m_pName, "A built-in function needs to have a name!");
    ezStringBuilder builder;
    builder.AppendFormat("<procedure (built-in):%s>", m_pName->value().GetData());
    return builder;
}

ezString
lcpp::LispFunction_BuiltIn::dump() const
{
    return toString();
}

lcpp::Ptr<lcpp::LispObject>
lcpp::LispFunction_BuiltIn::call(Ptr<LispObject> pArgList)
{
    EZ_ASSERT(m_pExec, "The executor MUST be valid!");
    RecursionCounter counter(LispRuntime::instance());
    return (*m_pExec)(m_pEnv, pArgList);
}

//////////////////////////////////////////////////////////////////////////

lcpp::Ptr<lcpp::LispFunction_UserDefined>
lcpp::LispFunction_UserDefined::create(Ptr<Environment> pEnv,
                                       Ptr<LispObject> pArgNameList,
                                       Ptr<LispCons> pBody)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    auto pNewEnv = Environment::create(pEnv);
    return LCPP_NEW(pAllocator, LispFunction_UserDefined)(pNewEnv, pArgNameList, pBody);
}

lcpp::Ptr<lcpp::LispFunction_UserDefined>
lcpp::LispFunction_UserDefined::copy(const LispFunction_UserDefined& toCopy)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    return LCPP_NEW(pAllocator, LispFunction_UserDefined)(toCopy);
}

const lcpp::Type&
lcpp::LispFunction_UserDefined::typeInfo()
{
    static auto t = Type::create(Type::Flags::Callable,
                                 EZ_STRINGIZE(LispFunction_UserDefined),
                                 "function (userdefined)",
                                 MemoryInfo(sizeof(LispFunction_UserDefined),
                                            sizeof(LispFunction_UserDefined)));

    return t;
}

lcpp::LispFunction_UserDefined::LispFunction_UserDefined(Ptr<Environment> pEnv,
                                                         Ptr<LispObject> pArgNameList,
                                                         Ptr<LispCons> pBody) :
    LispFunction(pEnv),
    m_pArgNameList(pArgNameList),
    m_numArgs(0),
    m_pBody(pBody)
#ifdef _DEBUG
    , m_dump(LispRuntime::instance()->allocator().get())
#endif // _DEBUG
{
    EZ_ASSERT(m_pArgNameList, "The function body MUST be valid argNameList!");
    EZ_ASSERT(m_pBody, "The function body MUST be valid!");

#ifdef _DEBUG
    {
        ezStringBuilder builder;

        m_pBody->toStringHelper(builder);
        m_dump = builder;
    }
#endif // _DEBUG

    if(isNil(m_pArgNameList))
    {
        return;
    }

    EZ_ASSERT(m_pArgNameList->is<LispCons>(), "Invalid arg name list.");

    auto pArgNamePtr = m_pArgNameList.cast<LispCons>();

    while(true)
    {
        EZ_ASSERT(pArgNamePtr->car()->is<LispSymbol>(), "All arg names must be symbols!");
        auto pArgName = pArgNamePtr->car().cast<LispSymbol>();
        m_pEnv->add(pArgName, LCPP_NIL);
        ++m_numArgs;

        if(isNil(pArgNamePtr->cdr()))
        {
            break;
        }
        EZ_ASSERT(pArgNamePtr->cdr()->is<LispCons>(), "Invalid arg name list.");
        pArgNamePtr = pArgNamePtr->cdr().cast<LispCons>();
    }
}

lcpp::Ptr<lcpp::LispObject>
lcpp::LispFunction_UserDefined::copy() const
{
    return copy(*this);
}

ezString
lcpp::LispFunction_UserDefined::toString() const
{
    static auto anonymous = ezString("<procedure>");
    static auto nonAnonymousFormat = ezString("<procedure:%s>");

    if (!m_pName)
    {
        return "<procedure>";
    }

    auto builder = ezStringBuilder();
    builder.AppendFormat("<procedure:%s>", m_pName->value().GetData());
    return builder;
}

ezString
lcpp::LispFunction_UserDefined::dump() const
{
    ezStringBuilder builder;
    builder.Append("(lambda ");
    builder.Append(m_pArgNameList->toString().GetData());
    builder.Append(' ');
    m_pBody->toStringHelper(builder);
    builder.Append(')');
    return builder;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::LispFunction_UserDefined::call(Ptr<LispObject> pArgList)
{
    EZ_ASSERT(m_pBody, "The function body MUST be valid!");
    RecursionCounter counter(LispRuntime::instance());

    // Process args
    //////////////////////////////////////////////////////////////////////////
    processArguments(pArgList);

    // Process body
    //////////////////////////////////////////////////////////////////////////
    Ptr<LispObject> pCodePointer = m_pBody;
    Ptr<LispObject> pResult = LCPP_NIL;

    while(!isNil(pCodePointer))
    {
        EZ_ASSERT(pCodePointer->is<LispCons>(), "Function body must be a cons.");

        auto pCons = pCodePointer.cast<LispCons>();
        pResult = LispRuntime::instance()->evaluator()->evalulate(m_pEnv, pCons->car());
        pCodePointer = pCons->cdr();
    }

    return pResult;
}

void
lcpp::LispFunction_UserDefined::processArguments(Ptr<LispObject> pArgs)
{
    if(!checkArgumentCount(pArgs)) { return; }

    // Update environment with argument values
    //////////////////////////////////////////////////////////////////////////
    EZ_ASSERT(pArgs->is<LispCons>(), "pArgs must be a cons if it is not nil!");

    auto pCurrentArgName = m_pArgNameList.cast<LispCons>();
    auto pCurrentArg = pArgs.cast<LispCons>();

    for(auto i = m_numArgs; i > 0; --i)
    {
        auto setResult = m_pEnv->set(pCurrentArgName->car().cast<LispSymbol>(),
                                   pCurrentArg->car());
        EZ_ASSERT(setResult.IsSuccess(),
                  "m_pArgNameList was changed after construction of this function object "
                  "or the environment was not properly set up when this function object was constructed.");

        pCurrentArgName = pCurrentArgName->cdr().cast<LispCons>();
        pCurrentArg = pCurrentArg->cdr().cast<LispCons>();
    }
}

bool
lcpp::LispFunction_UserDefined::checkArgumentCount(Ptr<LispObject> pArgs)
{
    if(isNil(pArgs))
    {
        // No args are given, but 1 or more are expected.
        if(m_numArgs > 0)
        {
            ezStringBuilder message;
            message.AppendFormat("Invalid number of arguments: expected %u, got none.", m_numArgs);
            throw exceptions::InvalidInput(message.GetData());
        }
        return false;
    }

    // From here on out we know that pArgs MUST be a cons.
    EZ_ASSERT(pArgs->is<LispCons>(), "pArgs must be a cons if it is not nil!");
    auto pArgList = pArgs.cast<LispCons>();

    // pArgs is not nil, which means it's a cons, so we count the args
    ezUInt32 numElements = 0;
    auto res = count(pArgList, numElements);
    EZ_ASSERT(res.IsSuccess(), "pArgList is not a regular list!");

    // pArgs is not nil but we don't expect any arguments
    if(m_numArgs == 0)
    {
        ezStringBuilder message;
        message.AppendFormat("Invalid number of arguments: expected no arguments, got %u", numElements);
        throw exceptions::InvalidInput(message.GetData());
    }

    if(numElements != m_numArgs)
    {
        ezStringBuilder message;
        message.AppendFormat("Invalid number of arguments: Expected %u arguments, got %u", m_numArgs, numElements);
        throw exceptions::InvalidInput(message.GetData());
    }

    return true;
}
