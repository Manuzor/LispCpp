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
            auto pStream = pStack->get(0);
            typeCheck(pStream, Type::Stream);

            LCPP_NOT_IMPLEMENTED;
        }

    }
}
