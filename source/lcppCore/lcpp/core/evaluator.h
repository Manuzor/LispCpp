#pragma once
#include "lcpp/core/environment.h"

namespace lcpp
{
    class LispRuntime;
    class LispObject;
    class LispCons;
    class Reader;

    class LCPP_CORE_API IEvaluator
    {
    public:
        virtual ~IEvaluator() {}

        virtual void initialize() = 0;

        virtual Ptr<LispObject> evalulate(Ptr<LispObject> pObject) = 0;
        virtual Ptr<LispObject> evalulate(Ptr<Environment> pEnv, Ptr<LispObject> pObject) = 0;
    };

    class LCPP_CORE_API RecursiveEvaluator : public IEvaluator
    {
    public:

        static Ptr<RecursiveEvaluator> create();

    public:

        virtual ~RecursiveEvaluator();

        virtual void initialize() LCPP_OVERRIDE;

        virtual Ptr<LispObject> evalulate(Ptr<LispObject> pObject) LCPP_OVERRIDE;
        virtual Ptr<LispObject> evalulate(Ptr<Environment> pEnv, Ptr<LispObject> pObject) LCPP_OVERRIDE;

    private:
        ezUInt32 m_evalLevel;

        RecursiveEvaluator();

        void evaluateEach(Ptr<Environment> pEnv, Ptr<LispCons> pCons);
    };
}

#include "lcpp/core/implementation/evaluator.inl"
