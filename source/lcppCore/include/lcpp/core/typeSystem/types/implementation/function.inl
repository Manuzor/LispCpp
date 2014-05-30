#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/evaluator.h"

inline
lcpp::SchemeFunction::SchemeFunction(Ptr<Environment> pParentEnv,
                                     Ptr<IEvaluator> pEvaluator,
                                     Ptr<SchemeCons> pBody) :
    m_env("function", pParentEnv),
    m_pEvaluator(pEvaluator),
    m_pBody(pBody),
    m_exec(nullptr)
{
}

inline
lcpp::SchemeFunction::SchemeFunction(Ptr<Environment> pParentEnv,
                                     Ptr<IEvaluator> pEvaluator,
                                     Executor exec) :
    m_env("builtin-function", pParentEnv),
    m_pEvaluator(pEvaluator),
    m_pBody(nullptr),
    m_exec(exec)
{
    EZ_ASSERT(exec, "The function executor must be valid!");
}

inline
bool
lcpp::SchemeFunction::operator==(const SchemeObject& obj) const
{
    if (obj.is<SchemeFunction>())
    {
        return *this == static_cast<const SchemeFunction&>(obj);
    }
    return false;
}

inline
bool
lcpp::SchemeFunction::operator==(const SchemeFunction& rhs) const
{
    return &rhs == this;
}

inline
ezString
lcpp::SchemeFunction::toString() const
{
    if(m_exec)
    {
        return "built-in function";
    }
    return m_pBody->toString();
}

inline
lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeFunction::call(Ptr<SchemeObject> pArgList)
{
    if (m_exec)
    {
        return m_exec(&m_env, pArgList);
    }
    EZ_ASSERT(m_pBody, "The body must NOT be invalid!");
    return execute(pArgList);
}

inline
lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeFunction::execute(Ptr<SchemeObject> pArgList)
{
    Ptr<SchemeObject> pResult = &SCHEME_VOID;
    // TODO Implement me.
    return pResult;
}
