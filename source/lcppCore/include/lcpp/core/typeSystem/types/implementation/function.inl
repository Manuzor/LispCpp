
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
                                     Ptr<SchemeCons> pBody,
                                     Executor exec) :
    m_env("builtin-function", pParentEnv),
    m_pEvaluator(pEvaluator),
    m_pBody(pBody),
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
    // TODO implement this properly!
    return m_pBody->toString();
}

inline
lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeFunction::operator()()
{
    if (m_exec)
    {
        return m_exec(&m_env, m_pBody);
    }
    
    return execute(&m_env, m_pBody);
}

inline
lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeFunction::execute(Ptr<Environment> pEnv, Ptr<SchemeCons> pBody)
{
    Ptr<SchemeObject> pResult = &SCHEME_VOID;
    // TODO Implement me.
    return pResult;
}
