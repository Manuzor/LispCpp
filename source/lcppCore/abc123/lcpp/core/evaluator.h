#pragma once
#include "lcpp/core/typeSystem/typeFactory.h"
#include "lcpp/core/environment.h"

namespace lcpp
{
    class SchemeRuntime;
    class SchemeObject;
    class Reader;

    class LCPP_CORE_API IEvaluator
    {
    public:
        virtual ~IEvaluator() {}

        virtual void initialize() = 0;

        virtual Ptr<SchemeObject> evalulate(Ptr<SchemeObject> pObject) = 0;
        virtual Ptr<SchemeObject> evalulate(Ptr<Environment> pEnv, Ptr<SchemeObject> pObject) = 0;

        virtual Ptr<SchemeRuntime> runtime() = 0;
        virtual Ptr<const SchemeRuntime> runtime() const = 0;
    };

    class LCPP_CORE_API RecursiveEvaluator : public IEvaluator
    {
        friend class SchemeRuntime;
    public:
        virtual ~RecursiveEvaluator();

        virtual void initialize() LCPP_OVERRIDE;

        virtual Ptr<SchemeObject> evalulate(Ptr<SchemeObject> pObject) LCPP_OVERRIDE;
        virtual Ptr<SchemeObject> evalulate(Ptr<Environment> pEnv, Ptr<SchemeObject> pObject) LCPP_OVERRIDE;

        virtual Ptr<SchemeRuntime> runtime() LCPP_OVERRIDE;
        virtual Ptr<const SchemeRuntime> runtime() const LCPP_OVERRIDE;

    private:
        Ptr<SchemeRuntime> m_pRuntime;

    private:
        ezUInt32 m_evalLevel;

        explicit RecursiveEvaluator(Ptr<SchemeRuntime> pRuntime);
        
        void evaluateEach(Ptr<Environment> pEnv, Ptr<SchemeCons> pCons);
    };
}

#include "lcpp/core/implementation/evaluator_inl.h"
