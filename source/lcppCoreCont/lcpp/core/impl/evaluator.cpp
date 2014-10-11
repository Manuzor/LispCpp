#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/attributeCheck.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/exceptions/evaluatorException.h"

#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/metaInfo.h"

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

            if(object::isType(pToEval, Type::Symbol))
            {
                auto pResult = LCPP_pNil;
                auto result = env::getBinding(pEnv, pToEval, pResult);

                if(result.Succeeded())
                {
                    LCPP_cont_return(pCont, pResult);
                }

                ezStringBuilder message;

                message.Format("No binding found for symbol '%s'.", symbol::getValue(pToEval).GetData());
                LCPP_THROW(exceptions::NoBindingFound(message.GetData()));
            }

            if(!object::isType(pToEval, Type::Cons))
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
                typeCheck(pEnv, Type::Environment);

                auto pUnevaluatedArgs = pStack->get(1);
                auto pToCall = pStack->get(2);

                if (!object::isCallable(pToCall))
                {
                    auto message = ezStringBuilder();
                    message.Format("Object of type %s is not callable!",
                                   object::getMetaInfo(pToCall).getPrettyName());
                    LCPP_THROW(exceptions::EvaluatorBase(message.GetData()));
                }

                // Pop everything from the stack.
                // This is needed so that the stack can be freely formed from now on
                // according to the needs of the then-called functions.
                pStack->clear();

                //////////////////////////////////////////////////////////////////////////

                // Push the current environment.
                pStack->push(pEnv);

                if(object::isType(pToCall, Type::Syntax))
                {
                    // Assumed syntax signature: <env> [<arg>...]
                    cons::pushAll(pUnevaluatedArgs, pStack);

                    // Push the object to call for lcpp::call
                    pStack->push(pToCall);
                    LCPP_cont_tailCall(pCont, &object::call);
                }

                // If it is not syntax, then it must be of Type::Lambda
                typeCheck(pToCall, Type::Lambda);
                
                // Assumed lambda signature: <unique-env> [<arg>...]

                // Push all arguments on the stack
                cons::pushAll(pUnevaluatedArgs, pStack);

                // Push the caller on the stack.
                pStack->push(pToCall);

                // Stack: <current-env> [<arg>...] <caller-env> <caller>

                cont::setUserData(pCont, 1);
                LCPP_cont_tailCall(pCont, &evaluateCallable_evalEach);
            }

            Ptr<LispObject> evaluateCallable_evalEach(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto index = cont::getUserData(pCont);

                // On top of the stack lies the callable object, which should not be evaluated.
                const auto maxIndex = pStack->size() - 2;

                if(index > maxIndex)
                {
                    LCPP_cont_tailCall(pCont, &evaluateCallable_call);
                }
                
                auto pToEval = pStack->get(index);

                cont::setFunction(pCont, &evaluateCallable_processEvaluatedArg);
                auto pEnv = pStack->get(0);
                LCPP_cont_call(pCont, &evaluate, pEnv, pToEval);
            }

            Ptr<LispObject> evaluateCallable_processEvaluatedArg(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto index = cont::getUserData(pCont);

                auto pEvaluatedArg = pStack->get(-1);
                pStack->pop();

                pStack->get(index) = pEvaluatedArg;

                cont::setUserData(pCont, index + 1);

                LCPP_cont_tailCall(pCont, evaluateCallable_evalEach);
            }

            Ptr<LispObject> evaluateCallable_call(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                auto pToCall = pStack->get(-1);
                auto pParentEnv = object::getEnvironment(pToCall);
                typeCheck(pParentEnv, Type::Environment);
                auto pCallerEnv = LCPP_pNil;

                if(object::hasName(pToCall))
                {
                    auto pName = object::getName(pToCall);
                    pCallerEnv = env::create(pParentEnv, pName);
                }
                else
                {
                    pCallerEnv = env::createAnonymous(pParentEnv);
                }

                typeCheck(pCallerEnv, Type::Environment);

                // Push the caller environment on the stack
                pStack->get(0) = pCallerEnv;

                LCPP_cont_tailCall(pCont, &object::call);
            }


        }
    }
}
