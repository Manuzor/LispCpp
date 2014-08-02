#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/exceptions/noBindingFoundException.h"

namespace lcpp
{
    namespace eval
    {

        Ptr<LispObject> evaluate(Ptr<LispObject> pContinuation)
        {
            typeCheck(pContinuation, Type::Continuation);

            auto pStack = cont::getStack(pContinuation);
            auto pEnv = pStack->get(0);
            auto pToEval = pStack->get(1);

            if (pToEval->isType(Type::Symbol))
            {
                auto pResult = LCPP_pNil;
                auto result = env::getBinding(pEnv, pToEval, pResult);

                if(result.Succeeded())
                {
                    LCPP_cont_return(pContinuation, pResult);
                }

                // TODO throw exception.
                auto message = ezStringBuilder();

                message.Format("No binding found for symbol '%s'.", symbol::getValue(pToEval).GetData());
                LCPP_THROW(exceptions::NoBindingFound(message.GetData()));
            }

            if (!pToEval->isType(Type::Cons))
            {
                // Object evaluates to itself.
                LCPP_cont_return(pContinuation, pToEval);
            }

            // TODO Syntax and lambdas...
            LCPP_NOT_IMPLEMENTED;
        }

    }
}
