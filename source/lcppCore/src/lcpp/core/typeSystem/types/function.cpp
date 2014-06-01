#include "stdafx.h"

#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/evaluator.h"

lcpp::SchemeFunction::SchemeFunction(const ezString& name,
                                     Ptr<Environment> pEnv) :
    m_name(name),
    m_pEnv(pEnv)
{
    m_pEnv->name() = m_name;
}

bool
lcpp::SchemeFunction::operator==(const SchemeObject& obj) const
{
    if (obj.is<SchemeFunction>())
    {
        return *this == static_cast<const SchemeFunction&>(obj);
    }
    return false;
}

bool
lcpp::SchemeFunction::operator==(const SchemeFunction& rhs) const
{
    // Identity check.
    return &rhs == this;
}

//////////////////////////////////////////////////////////////////////////

lcpp::SchemeFunctionBuiltin::SchemeFunctionBuiltin(const ezString& name, Ptr<Environment> pEnv, Executor exec) :
SchemeFunction(name, pEnv),
    m_exec(exec)
{
    EZ_ASSERT(!name.IsEmpty(), "A builtin function needs a name!");

    ezStringBuilder builder;
    builder.AppendFormat("builtin-procedure:%s", m_name.GetData());
    m_pEnv->name() = builder;

    EZ_ASSERT(exec, "The function executor must be valid!");
}

ezString
lcpp::SchemeFunctionBuiltin::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("<builtin-procedure:%s>", m_name.GetData());
    return builder;
}

ezString
lcpp::SchemeFunctionBuiltin::dump() const
{
    return toString();
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeFunctionBuiltin::call(Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgList)
{
    EZ_ASSERT(m_exec, "The executor MUST be valid!");
    return m_exec(m_pEnv, pEvaluator, pArgList);
}

//////////////////////////////////////////////////////////////////////////

lcpp::SchemeFunctionUserDefined::SchemeFunctionUserDefined(Ptr<Environment> pEnv,
                                                           Ptr<SchemeObject> pArgNameList,
                                                           Ptr<SchemeCons> pBody) :
    SchemeFunction("anonymous", pEnv),
    m_pArgNameList(pArgNameList),
    m_numArgs(0),
    m_pBody(pBody)
#ifdef _DEBUG
    ,m_dump("")
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

    EZ_ASSERT(m_pArgNameList->is<SchemeCons>(), "Invalid arg name list.");

    auto pArgNamePtr = m_pArgNameList.cast<SchemeCons>();

    while(true)
    {
        EZ_ASSERT(pArgNamePtr->car()->is<SchemeSymbol>(), "All arg names must be symbols!");
        auto pArgName = pArgNamePtr->car().cast<SchemeSymbol>();
        m_pEnv->add(pArgName, SCHEME_NIL_PTR);
        ++m_numArgs;

        if(isNil(pArgNamePtr->cdr()))
        {
            break;
        }
        EZ_ASSERT(pArgNamePtr->cdr()->is<SchemeCons>(), "Invalid arg name list.");
        pArgNamePtr = pArgNamePtr->cdr().cast<SchemeCons>();
    }
}

ezString
lcpp::SchemeFunctionUserDefined::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("<procedure:%s>", m_name.GetData());
    return builder;
}

ezString
lcpp::SchemeFunctionUserDefined::dump() const
{
    ezStringBuilder builder;
    builder.Append("(lambda ");
    builder.Append(m_pArgNameList->toString().GetData());
    builder.Append(' ');
    m_pBody->toStringHelper(builder);
    builder.Append(')');
    return builder;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeFunctionUserDefined::call(Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgList)
{
    EZ_ASSERT(m_pBody, "The function body MUST be valid!");

    // TODO Process args
    //////////////////////////////////////////////////////////////////////////
    processArguments(pArgList);

    // Process body
    //////////////////////////////////////////////////////////////////////////
    Ptr<SchemeObject> pCodePointer = m_pBody;
    Ptr<SchemeObject> pResult = SCHEME_NIL_PTR;

    while(!isNil(pCodePointer))
    {
        EZ_ASSERT(pCodePointer->is<SchemeCons>(), "Function body must be a cons.");

        auto pCons = pCodePointer.cast<SchemeCons>();
        pResult = pEvaluator->evalulate(m_pEnv, pCons->car());
        pCodePointer = pCons->cdr();
    }

    return pResult;
}

void
lcpp::SchemeFunctionUserDefined::processArguments(Ptr<SchemeObject> pArgs)
{
    if(!checkArgumentCount(pArgs)) { return; }

    // Update environment with argument values
    //////////////////////////////////////////////////////////////////////////
    EZ_ASSERT(pArgs->is<SchemeCons>(), "pArgs must be a cons if it is not nil!");

    auto pCurrentArgName = m_pArgNameList.cast<SchemeCons>();
    auto pCurrentArg = pArgs.cast<SchemeCons>();

    for(auto i = m_numArgs; i > 0; --i)
    {
        auto setResult = m_pEnv->set(pCurrentArgName->car().cast<SchemeSymbol>(),
                                   pCurrentArg->car());
        EZ_ASSERT(setResult.IsSuccess(),
                  "m_pArgNameList was changed after construction of this function object "
                  "or the environment was not properly set up when this function object was constructed.");

        pCurrentArgName = pCurrentArgName->cdr().cast<SchemeCons>();
        pCurrentArg = pCurrentArg->cdr().cast<SchemeCons>();
    }
}

bool
lcpp::SchemeFunctionUserDefined::checkArgumentCount(Ptr<SchemeObject> pArgs)
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
    EZ_ASSERT(pArgs->is<SchemeCons>(), "pArgs must be a cons if it is not nil!");
    auto pArgList = pArgs.cast<SchemeCons>();

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
