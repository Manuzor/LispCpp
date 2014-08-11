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

#include "lcpp/core/exceptions/invalidInputException.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"

#include "lcpp/core/exceptions/noBindingFoundException.h"

// Provides the variables pStack and pEnv in the current context.
#define LCPP_SyntaxBuiltinFunction_CommonBody \
    typeCheck(pCont, Type::Continuation);     \
                                              \
    auto pStack = cont::getStack(pCont);      \
                                              \
    auto pEnv = pStack->get(0);               \
    typeCheck(pEnv, Type::Environment)

namespace lcpp
{
    namespace syntax
    {
        namespace builtin
        {
            Ptr<LispObject> if_(Ptr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pCondition = pStack->get(1);

                cont::setFunction(pCont, &detail::if_helper);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pCondition);
            }

            Ptr<LispObject> detail::if_helper(Ptr<LispObject> pCont)
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

            Ptr<LispObject> define(Ptr<LispObject> pCont)
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
                    auto pArgNameList = cons::getCdr(pFirstArg);

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

            Ptr<LispObject> detail::define_addBinding(Ptr<LispObject> pCont)
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

            Ptr<LispObject> set(Ptr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pSymbol = LCPP_pNil;
                auto pFirstArg = pStack->get(1);

                if (object::isType(pFirstArg, Type::Cons))
                {
                    pSymbol = cons::getCar(pFirstArg);
                }
                else
                {
                    pSymbol = pFirstArg;
                }
                
                auto bindingLocation = env::existsBinding(pEnv, pSymbol);

                if(bindingLocation.doesNotExist())
                {
                    auto message = ezStringBuilder();
                    message.Format("Cannot set symbol \"%s\" before its definition.", symbol::getValue(pSymbol).GetData());
                    LCPP_THROW(exceptions::NoBindingFound(message.GetData()));
                }

                LCPP_cont_tailCall(pCont, &define);
            }

            Ptr<LispObject> begin(Ptr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                cont::setUserData(pCont, 1);

                pStack->push(LCPP_pVoid);

                LCPP_cont_tailCall(pCont, &detail::begin_helper);
            }

            Ptr<LispObject> detail::begin_helper(Ptr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pEvalResult = pStack->get(-1);
                pStack->pop();

                const auto maxIndex = pStack->size();
                auto& index = cont::getUserData(pCont);

                if (index >= maxIndex)
                {
                    LCPP_cont_return(pCont, pEvalResult);
                }

                auto pToEval = pStack->get(ezInt32(index));
                ++index;

                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pToEval);
            }

            Ptr<LispObject> lambda(Ptr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pArgNameList = pStack->get(1);
                auto pBodyList = cons::pack(pStack, 2);

                auto pLambda = lcpp::lambda::userDefined::create(pEnv, pArgNameList, pBodyList);
                LCPP_cont_return(pCont, pLambda);
            }

            Ptr<LispObject> quote(Ptr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pUnevaluatedArg = pStack->get(1);

                LCPP_cont_return(pCont, pUnevaluatedArg);
            }

            Ptr<LispObject> and(Ptr<LispObject> pCont)
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

            Ptr<LispObject> detail::and_helper(Ptr<LispObject> pCont)
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

            Ptr<LispObject> or(Ptr<LispObject> pCont)
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

            Ptr<LispObject> detail::or_helper(Ptr<LispObject> pCont)
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
        }
    }
}

#undef LCPP_SyntaxBuiltinFunction_CommonBody
