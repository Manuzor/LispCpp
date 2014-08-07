#include "stdafx.h"
#include "lcpp/core/builtins/syntax_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/continuation.h"

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/void.h"

#include "lcpp/core/exceptions/invalidInputException.h"
#include "lcpp/core/typeSystem/object.h"

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
            Ptr<LispObject> define(Ptr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pSymbol = pStack->get(1);
                typeCheck(pSymbol, Type::Symbol);

                auto pValue = pStack->get(2);
                // Pop the unevaluated value so eval::evaluate can push the evaluated value.
                pStack->pop();

                //////////////////////////////////////////////////////////////////////////

                cont::setFunction(pCont, &detail::define_2);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pValue);
            }

            LCPP_API_CORE_CONT Ptr<LispObject> begin(Ptr<LispObject> pCont)
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

            Ptr<LispObject> detail::define_2(Ptr<LispObject> pCont)
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

            Ptr<LispObject> lambda(Ptr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pArgNameList = pStack->get(0);

                LCPP_NOT_IMPLEMENTED;
            }

            Ptr<LispObject> quote(Ptr<LispObject> pCont)
            {
                LCPP_SyntaxBuiltinFunction_CommonBody;

                auto pUnevaluatedArg = pStack->get(1);

                LCPP_cont_return(pCont, pUnevaluatedArg);
            }

        }
    }
}

#undef LCPP_SyntaxBuiltinFunction_CommonBody
