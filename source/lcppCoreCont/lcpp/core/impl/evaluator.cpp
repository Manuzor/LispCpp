#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectUtils.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/exceptions/noBindingFoundException.h"

namespace lcpp
{
    namespace eval
    {

        Ptr<LispObject> evaluate(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            auto pStack = cont::getStack(pCont);

            auto pEnv = pStack->get(0);
            typeCheck(pEnv, Type::Environment);

            auto pToEval = pStack->get(1);

            //////////////////////////////////////////////////////////////////////////

            if (pToEval->isType(Type::Symbol))
            {
                auto pResult = LCPP_pNil;
                auto result = env::getBinding(pEnv, pToEval, pResult);

                if(result.Succeeded())
                {
                    LCPP_cont_return(pCont, pResult);
                }

                // TODO throw exception.
                auto message = ezStringBuilder();

                message.Format("No binding found for symbol '%s'.", symbol::getValue(pToEval).GetData());
                LCPP_THROW(exceptions::NoBindingFound(message.GetData()));
            }

            if (!pToEval->isType(Type::Cons))
            {
                // Object evaluates to itself.
                LCPP_cont_return(pCont, pToEval);
            }

            auto pToCall = cons::getCar(pToEval);

            cont::setFunction(pCont, &detail::evaluateCallable);
            LCPP_cont_call(pCont, &evaluate, pEnv, pToCall);
        }

        namespace detail
        {
            Ptr<LispObject> evaluateCallable(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto pEnv = pStack->get(0);
                auto pToEval = pStack->get(1);
                auto pToCall = pStack->get(2);

                // Pop pToCall from the stack and push it on top again later, after all arguments are pushed on the stack.
                pStack->pop();

                if (pToCall->isType(Type::Syntax))
                {
                    // Pop pToEval from the stack
                    pStack->pop();

                    auto pArgs = cons::getCdr(pToEval);
                    cons::pushAll(pArgs, pStack);

                    // Push the object to call for lcpp::call
                    pStack->push(pToCall);
                    LCPP_cont_tailCall(pCont, &call);
                }
                
                LCPP_NOT_IMPLEMENTED;
            }
        }
    }
}
