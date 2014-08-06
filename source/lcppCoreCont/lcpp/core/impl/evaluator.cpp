#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectUtils.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/attributeCheck.h"

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
            pStack->pop();

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
            auto pUnevaluatedArgs = cons::getCdr(pToEval);
            pStack->push(pUnevaluatedArgs);

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
                auto pUnevaluatedArgs = pStack->get(1);
                auto pToCall = pStack->get(2);
                attributeCheckAny(pToCall, AttributeFlags::Callable);

                // Pop everything from the stack.
                // This is needed so that the stack can be freely formed from now on
                // according to the needs of the then-called functions.
                pStack->clear();

                //////////////////////////////////////////////////////////////////////////

                if (pToCall->isType(Type::Syntax))
                {
                    // Assumed syntax signature: <env> [<arg>...]

                    pStack->push(pEnv);
                    cons::pushAll(pUnevaluatedArgs, pStack);

                    // Push the object to call for lcpp::call
                    pStack->push(pToCall);
                    LCPP_cont_tailCall(pCont, &call);
                }

                // If it is not syntax, then it must be of Type::Lambda
                typeCheck(pToCall, Type::Lambda);
                
                // Assumed lambda signature: [<arg>...]

                // TODO copy arguments?

                cons::pushAll(pUnevaluatedArgs, pStack);
                pStack->push(pToCall);
                pStack->push(pEnv); // Needed for calls to evaluate.
                LCPP_cont_tailCall(pCont, &evaluateCallable_evalEach);
            }

            Ptr<LispObject> evaluateCallable_evalEach(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto index = ezInt32(cont::getUserData(pCont));

                // On top of the stack lies the callable object, which should not be evaluated.
                const auto maxIndex = pStack->size() - 2;

                if(index == maxIndex)
                {
                    // Pop pEnv from the stack; it is no longer needed.
                    pStack->pop();
                    LCPP_cont_tailCall(pCont, &call);
                }
                
                auto pArg = pStack->get(index);

                cont::setFunction(pCont, &evaluateCallable_processEvaluatedArg);
                auto pEnv = pStack->get(-1);
                LCPP_cont_call(pCont, &evaluate, pEnv, pArg);
            }

            Ptr<LispObject> evaluateCallable_processEvaluatedArg(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto index = ezInt32(cont::getUserData(pCont));

                auto pEvaluatedArg = pStack->get(-1);
                pStack->pop();

                pStack->get(index) = pEvaluatedArg;

                cont::setUserData(pCont, index + 1);

                LCPP_cont_tailCall(pCont, evaluateCallable_evalEach);
            }

        }
    }
}
