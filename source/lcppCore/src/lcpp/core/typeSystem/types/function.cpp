#include "stdafx.h"

#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/evaluator.h"

lcpp::SchemeFunction::SchemeFunction(const ezString& name,
                                     Ptr<Environment> pParentEnv) :
    m_name(name),
    m_env("", pParentEnv)
{
    if(m_name.IsEmpty())
    {
        m_name = "anonymous-procedure";
    }
    m_env.name() = m_name;
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

lcpp::SchemeFunctionBuiltin::SchemeFunctionBuiltin(const ezString& name, Ptr<Environment> pParentEnv, Executor exec) :
    SchemeFunction(name, pParentEnv),
    m_exec(exec)
{
    EZ_ASSERT(!name.IsEmpty(), "A builtin function needs a name!");

    ezStringBuilder builder;
    builder.AppendFormat("builtin-procedure:%s", m_name.GetData());
    m_env.name() = builder;

    EZ_ASSERT(exec, "The function executor must be valid!");
}

ezString
lcpp::SchemeFunctionBuiltin::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("<builtin-procedure:%s>", m_name.GetData());
    return builder;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeFunctionBuiltin::call(Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgList)
{
    EZ_ASSERT(m_exec, "The executor MUST be valid!");
    return m_exec(&m_env, pEvaluator, pArgList);
}

//////////////////////////////////////////////////////////////////////////

lcpp::SchemeFunctionUserDefined::SchemeFunctionUserDefined(const ezString& name, Ptr<Environment> pParentEnv, Ptr<SchemeCons> pBody) :
    SchemeFunction(name, pParentEnv),
    m_pBody(pBody)
{
    EZ_ASSERT(!name.IsEmpty(), "A builtin function needs a name!");

    ezStringBuilder builder;
    builder.AppendFormat("procedure:%s", m_name.GetData());
    m_env.name() = builder;

    EZ_ASSERT(m_pBody, "The function body MUST be valid!");
}

ezString
lcpp::SchemeFunctionUserDefined::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("<procedure:%s>", m_name.GetData());
    return builder;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeFunctionUserDefined::call(Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgList)
{
    EZ_ASSERT(m_pBody, "The function body MUST be valid!");
    // TODO process args
    return pEvaluator->evalulate(&m_env, m_pBody);
}


