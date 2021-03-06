#include "stdafx.h"
#include "lcpp/core/builtins/syntax_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/continuation.h"

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/lambda_userDefined.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/number.h"

#include "lcpp/core/exceptions/invalidInputException.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"

#include "lcpp/core/exceptions/evaluatorException.h"
#include "lcpp/core/typeSystem/types/time.h"
#include "lcpp/core/printer.h"
#include "lcpp/core/exceptions/assertException.h"
#include "lcpp/core/typeSystem/types/string.h"

// Provides the variables pStack and pEnv in the current context.
#define LCPP_SyntaxBuiltinFunction_CommonBody   \
    typeCheck(pCont, Type::Continuation);       \
                                                \
    auto pStack = cont::getStack(pCont);        \
                                                \
    StackPtr<LispObject> pEnv = pStack->get(0); \
    typeCheck(pEnv, Type::Environment)

namespace lcpp
{
    namespace syntax
    {
        namespace builtin
        {
            Ptr<LispObject> if_(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pCondition = pStack->get(1);

                cont::setFunction(pCont, &detail::if_helper);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pCondition);
            }

            Ptr<LispObject> detail::if_helper(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pEvaluatedCondition = pStack->get(-1);
                auto pCaseTrue = pStack->get(2);
                auto pCaseFalse = pStack->get(3);

                pStack->clear();
                pStack->push(pEnv);

                if (isTrue(pEvaluatedCondition))
                {
                    pStack->push(pCaseTrue);
                }
                else
                {
                    pStack->push(pCaseFalse);
                }

                LCPP_cont_tailCall(pCont, &eval::evaluate);
            }

            Ptr<LispObject> define(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto argCount = pStack->size() - 1; // The env.

                auto pSymbol = LCPP_pNil;
                auto pFirstArg = pStack->get(1);

                // Lambda shorthand syntax
                //////////////////////////////////////////////////////////////////////////
                if (object::isType(pFirstArg, Type::Cons))
                {
                    pSymbol = cons::getCar(pFirstArg);
                    StackPtr<LispObject> pArgNameList = cons::getCdr(pFirstArg);

                    auto pContCall = cont::create(pCont, &lambda);
                    auto pStackCall = cont::getStack(pContCall);

                    // Push the pEnv, the argument name list, and all other arguments to define.
                    pStackCall->push(pEnv);
                    pStackCall->push(pArgNameList);
                    for(auto i = ezUInt32(2); i < pStack->size(); ++i)
                    {
                        auto pToPush = pStack->get(i);
                        pStackCall->push(pToPush);
                    }

                    // prepare our own stack for define_addBinding.
                    pStack->clear();
                    pStack->push(pEnv);
                    pStack->push(pSymbol);
                    // The call to &lambda will push the actual value on top.

                    // Set the proper function in pContCall's parent env pCont.
                    cont::setFunction(pCont, &detail::define_addBinding);
                    LCPP_cont_jump(pContCall);
                }

                // "Regular" define.
                //////////////////////////////////////////////////////////////////////////
                typeCheck(pFirstArg, Type::Symbol);

                if(argCount > 2) // env, key, value
                {
                    auto message = ezStringBuilder();
                    message.Format("Expected 2 arguments, got %u.", pStack->size());
                    LCPP_THROW(exceptions::ArgumentCount(message.GetData()));
                }

                auto pToDefine = pStack->get(2);
                // Pop the unevaluated value (pToDefine).
                pStack->pop();

                cont::setFunction(pCont, &detail::define_addBinding);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pToDefine);
            }

            Ptr<LispObject> detail::define_addBinding(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pSymbol = pStack->get(1);
                typeCheck(pSymbol, Type::Symbol);

                auto pValue = pStack->get(2);

                //////////////////////////////////////////////////////////////////////////

                env::addBinding(pEnv, pSymbol, pValue);

                // TODO give pValue a name, if it is nameable.

                if (object::isNameable(pValue) && !object::hasName(pValue))
                {
                    object::setName(pValue, pSymbol);
                }

                LCPP_cont_return(pCont, LCPP_pVoid);
            }

            static Ptr<LispObject> set_helper(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                StackPtr<LispObject> pSymbol = pStack->get(1);
                StackPtr<LispObject> pValue = pStack->get(2);

                if (env::setBinding(pEnv, pSymbol, pValue).Failed())
                {
                    ezStringBuilder message;
                    message.Format("Cannot set symbol \"%s\" before its definition.", symbol::getValue(pSymbol).GetData());
                    LCPP_THROW(exceptions::NoBindingFound(message.GetData()));
                }

                LCPP_cont_return(pCont, LCPP_pVoid);
            }

            Ptr<LispObject> set(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto argCount = pStack->size() - 1; // The env.

                if(argCount > 2) // env, key, value
                {
                    ezStringBuilder message;
                    message.Format("Expected 2 arguments, got %u.", pStack->size());
                    LCPP_THROW(exceptions::ArgumentCount(message.GetData()));
                }

                StackPtr<LispObject> pUnevaluatedArg = pStack->get(2);
                pStack->pop();
                cont::setFunction(pCont, &set_helper);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pUnevaluatedArg);
            }

            Ptr<LispObject> begin(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                if (pStack->size() == 1)
                {
                    LCPP_cont_return(pCont, LCPP_pVoid);
                }

                cont::setUserData(pCont, 1);

                pStack->push(LCPP_pVoid);

                LCPP_cont_tailCall(pCont, &detail::begin_helper);
            }

            Ptr<LispObject> detail::begin_helper(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                pStack->pop();

                const auto maxIndex = pStack->size();
                auto& index = cont::getUserData(pCont);

                auto pToEval = pStack->get(ezInt32(index));
                ++index;

                if (index == maxIndex)
                {
                    pStack->clear();
                    pStack->push(pEnv);
                    pStack->push(pToEval);
                    LCPP_cont_tailCall(pCont, &eval::evaluate);
                }

                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pToEval);
            }

            Ptr<LispObject> lambda(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                StackPtr<LispObject> pArgNameList = pStack->get(1);
                StackPtr<LispObject> pBodyList = cons::pack(pCont, 2);

                auto pLambda = lcpp::lambda::userDefined::create(pEnv, pArgNameList, pBodyList);
                LCPP_cont_return(pCont, pLambda);
            }

            Ptr<LispObject> quote(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pUnevaluatedArg = pStack->get(1);

                LCPP_cont_return(pCont, pUnevaluatedArg);
            }

            Ptr<LispObject> and(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                if (pStack->size() == 1)
                {
                    LCPP_cont_return(pCont, LCPP_pTrue);
                }

                cont::setUserData(pCont, 1);

                auto pFirstToEval = pStack->get(1);

                cont::setFunction(pCont, &detail::and_helper);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pFirstToEval);
            }

            Ptr<LispObject> detail::and_helper(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto& index = cont::getUserData(pCont);
                const auto maxIndex = pStack->size() - 2;

                auto pCurrentResult = pStack->get(-1);

                if(index >= maxIndex || isFalse(pCurrentResult))
                {
                    LCPP_cont_return(pCont, pCurrentResult);
                }

                ++index;
                auto pToEval = pStack->get(index);

                // Pop the current result from the stack so that eval::evaluate can push the new result.
                pStack->pop();

                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pToEval);
            }

            Ptr<LispObject> or(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                if(pStack->size() == 1)
                {
                    LCPP_cont_return(pCont, LCPP_pFalse);
                }

                cont::setUserData(pCont, 1);

                auto pFirstToEval = pStack->get(1);

                cont::setFunction(pCont, &detail::or_helper);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pFirstToEval);
            }

            Ptr<LispObject> detail::or_helper(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto& index = cont::getUserData(pCont);
                const auto maxIndex = pStack->size() - 2;

                auto pCurrentResult = pStack->get(-1);

                if(index >= maxIndex || isTrue(pCurrentResult))
                {
                    LCPP_cont_return(pCont, pCurrentResult);
                }

                ++index;
                auto pToEval = pStack->get(index);

                // Pop the current result from the stack so that eval::evaluate can push the new result.
                pStack->pop();

                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pToEval);
            }

            Ptr<LispObject> time(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pToCall = pStack->get(1);
                auto pTime = lcpp::time::create();

                pStack->push(pTime);

                // Continue with detail::time_finalize.
                cont::setFunction(pCont, &detail::time_finalize);

                // Assign it as late as possible.
                lcpp::time::setNow(pTime);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pToCall);
            }

            Ptr<LispObject> detail::time_finalize(StackPtr<LispObject> pCont)
            {
                auto endTime = ezTime::Now();

                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pTime = pStack->get(2);
                typeCheck(pTime, Type::Time);

                auto startTime = time::getTime(pTime);
                auto deltaTime = endTime - startTime;
                time::setTime(pTime, deltaTime);

                LCPP_cont_return(pCont, pTime);
            }

            Ptr<LispObject> assertion(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pUnevaluatedCondition = pStack->get(1);

                auto argCount = pStack->size() - 1;
                if (argCount == 1)
                {
                    // If the user did not specify a message, write the default message.
                    pStack->push(LCPP_pNil);
                }

                cont::setFunction(pCont, &detail::assertion_finalize);

                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pUnevaluatedCondition);
            }

            Ptr<LispObject> detail::assertion_finalize(StackPtr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pUnevaluatedCondition = pStack->get(1);
                auto pUserMessage = pStack->get(2);
                auto pCondition = pStack->get(3);

                if(isFalse(pCondition))
                {
                    ezStringBuilder message("Assertion failed.\n  Expression: ");

                    auto pConditionString = object::toString(pUnevaluatedCondition);
                    message.Append(str::getValue(pConditionString).GetData());

                    if (!isNil(pUserMessage))
                    {
                        message.AppendFormat("\n  Message: %s", str::getValue(pUserMessage).GetData());
                    }

                    LCPP_THROW(exceptions::UserAssertionFailed(message.GetData()));
                }

                LCPP_cont_return(pCont, pCondition);
            }

        }
    }
}

#undef LCPP_SyntaxBuiltinFunction_CommonBody
