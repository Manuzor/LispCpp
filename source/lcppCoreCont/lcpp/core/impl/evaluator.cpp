#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace eval
    {

        Ptr<LispObject> evaluate(Ptr<LispObject> pContinuation)
        {
            typeCheck(pContinuation, Type::Continuation);

            auto pStack = cont::getStack(pContinuation);
            auto pToEval = pStack->get(0);

            LCPP_cont_return(pContinuation, pToEval);
        }

    }
}
