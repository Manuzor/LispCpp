#pragma once
#include "lcpp/core/typeSystem/typeFactory.h"
#include "lcpp/core/environment.h"

namespace lcpp
{
    class LispRuntime;
    class LispObject;
    class Reader;

    class LCPP_CORE_API IEvaluator
    {
    public:
        virtual ~IEvaluator() {}

        virtual void initialize() = 0;

        virtual Ptr<LispObject> evalulate(Ptr<LispObject> pObject) = 0;
        virtual Ptr<LispObject> evalulate(Ptr<Environment> pEnv, Ptr<LispObject> pObject) = 0;

        virtual Ptr<LispRuntime> runtime() = 0;
        virtual Ptr<const LispRuntime> runtime() const = 0;
    };

    class LCPP_CORE_API RecursiveEvaluator : public IEvaluator
    {
        friend class LispRuntime;
    public:
        virtual ~RecursiveEvaluator();

        virtual void initialize() LCPP_OVERRIDE;

        virtual Ptr<LispObject> evalulate(Ptr<LispObject> pObject) LCPP_OVERRIDE;
        virtual Ptr<LispObject> evalulate(Ptr<Environment> pEnv, Ptr<LispObject> pObject) LCPP_OVERRIDE;

        virtual Ptr<LispRuntime> runtime() LCPP_OVERRIDE;
        virtual Ptr<const LispRuntime> runtime() const LCPP_OVERRIDE;

    private:
        Ptr<LispRuntime> m_pRuntime;

    private:
        ezUInt32 m_evalLevel;

        explicit RecursiveEvaluator(Ptr<LispRuntime> pRuntime);
        
        void evaluateEach(Ptr<Environment> pEnv, Ptr<LispCons> pCons);
    };
}

#include "lcpp/core/implementation/evaluator_inl.h"
