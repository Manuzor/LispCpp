#include "stdafx.h"

#include "lcpp/core/typeSystem.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/recursionCounter.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::Ptr<lcpp::LispFunction>
lcpp::LispFunction::create(const ezString& name,
                           Ptr<Environment> pEnv)
{
    EZ_REPORT_FAILURE("Not implemented");
    return nullptr;
}

const lcpp::Type&
lcpp::LispFunction::typeInfo()
{
    static auto t = Type::create(EZ_STRINGIZE(LispFunction),
                                 "function (base)",
                                 MemoryInfo(sizeof(LispFunction),
                                            sizeof(LispFunction)));

    return t;
}

lcpp::LispFunction::LispFunction(Ptr<LispRuntime> pRuntime,
                                     const ezString& name,
                                     Ptr<Environment> pEnv) :
    m_name(name),
    m_pRuntime(pRuntime),
    m_pEnv(pEnv)
{
    m_pEnv->name() = m_name;
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
lcpp::LispFunction::name(const ezString& newName)
{
    m_name = newName;
    m_pEnv->name() = newName;
}

//////////////////////////////////////////////////////////////////////////

lcpp::LispFunctionBuiltin::LispFunctionBuiltin(Ptr<LispRuntime> pRuntime,
                                                   const ezString& name,
                                                   Ptr<Environment> pEnv,
                                                   ExecutorPtr_t pExec) :
    LispFunction(pRuntime, name, pEnv),
    m_pExec(pExec)
{
    EZ_ASSERT(!name.IsEmpty(), "A builtin function needs a name!");

    ezStringBuilder builder(m_pRuntime->allocator().get());
    builder.AppendFormat("builtin-procedure:%s", m_name.GetData());
    m_pEnv->name() = builder;

    EZ_ASSERT(pExec, "The function executor must be valid!");
}

lcpp::Ptr<lcpp::LispObject>
lcpp::LispFunctionBuiltin::clone(ezAllocatorBase* pAllocator) const
{
    return LCPP_NEW(pAllocator, LispFunctionBuiltin)(*this);
}

ezString
lcpp::LispFunctionBuiltin::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("<builtin-procedure:%s>", m_name.GetData());
    return builder;
}

ezString
lcpp::LispFunctionBuiltin::dump() const
{
    return toString();
}

lcpp::Ptr<lcpp::LispObject>
lcpp::LispFunctionBuiltin::call(Ptr<LispObject> pArgList)
{
    EZ_ASSERT(m_pExec, "The executor MUST be valid!");
    RecursionCounter counter(m_pRuntime);
    return (*m_pExec)(m_pRuntime, m_pEnv, pArgList);
}

//////////////////////////////////////////////////////////////////////////

lcpp::LispFunctionUserDefined::LispFunctionUserDefined(Ptr<LispRuntime> pRuntime,
                                                           Ptr<Environment> pEnv,
                                                           Ptr<LispObject> pArgNameList,
                                                           Ptr<LispCons> pBody) :
    LispFunction(pRuntime, "anonymous", pEnv),
    m_pArgNameList(pArgNameList),
    m_numArgs(0),
    m_pBody(pBody)
#ifdef _DEBUG
    ,m_dump(m_pRuntime->allocator().get())
#endif // _DEBUG
{
    ezStringBuilder builder;
    builder.AppendFormat("procedure:%s", m_name.GetData());
    m_pEnv->name() = builder;

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
lcpp::LispFunctionUserDefined::clone(ezAllocatorBase* pAllocator) const
{
    return LCPP_NEW(pAllocator, LispFunctionUserDefined)(*this);
}

ezString
lcpp::LispFunctionUserDefined::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("<procedure:%s>", m_name.GetData());
    return builder;
}

ezString
lcpp::LispFunctionUserDefined::dump() const
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
lcpp::LispFunctionUserDefined::call(Ptr<LispObject> pArgList)
{
    EZ_ASSERT(m_pBody, "The function body MUST be valid!");
    RecursionCounter counter(m_pRuntime);

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
        pResult = m_pRuntime->evaluator()->evalulate(m_pEnv, pCons->car());
        pCodePointer = pCons->cdr();
    }

    return pResult;
}

void
lcpp::LispFunctionUserDefined::processArguments(Ptr<LispObject> pArgs)
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
lcpp::LispFunctionUserDefined::checkArgumentCount(Ptr<LispObject> pArgs)
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
