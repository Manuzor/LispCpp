#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/typeSystem/types/cons.h"

namespace lcpp
{
    class IEvaluator;

    class SchemeFunction : public SchemeExtend<SchemeFunction, SchemeObject>
    {
        friend class TypeFactory;
    public:
        typedef std::function<Ptr<SchemeObject>(Ptr<Environment>, Ptr<SchemeCons>)> Executor;

        SchemeFunction(Ptr<Environment> pParentEnv,
                       Ptr<IEvaluator> pEvaluator,
                       Ptr<SchemeCons> pBody);

        SchemeFunction(Ptr<Environment> pParentEnv,
                       Ptr<IEvaluator> pEvaluator,
                       Ptr<SchemeCons> pBody,
                       Executor exec);

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeFunction& rhs) const;

        virtual ezString toString() const LCPP_OVERRIDE;

        Ptr<SchemeObject> operator()();

    private:
        Environment m_env;
        Ptr<IEvaluator> m_pEvaluator;
        Ptr<SchemeCons> m_pBody;
        Executor m_exec;

        Ptr<SchemeObject> execute(Ptr<Environment> pEnv, Ptr<SchemeCons> pBody);
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeFunction);
}

#include "lcpp/core/typeSystem/types/implementation/function.inl"
