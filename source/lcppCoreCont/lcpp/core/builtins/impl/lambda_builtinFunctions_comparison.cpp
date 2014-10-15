#include "stdafx.h"
#include "lcpp/core/builtins/lambda_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/bool.h"

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"

#define LCPP_lambda_builtinFunctions_defineComparisonFunctionHelperContent(operatorFunction) \
    typeCheck(pCont, Type::Continuation);                                                    \
    auto pStack = cont::getStack(pCont);                                                     \
    auto& index = cont::getUserData(pCont);                                                  \
                                                                                             \
    const auto maxIndex = pStack->size() - 2;                                                \
                                                                                             \
    auto& pResult = pStack->get(-1);                                                         \
                                                                                             \
    if(index >= maxIndex)                                                                    \
    {                                                                                        \
        LCPP_cont_return(pCont, pResult);                                                    \
    }                                                                                        \
                                                                                             \
    auto pLhs = pStack->get(index);                                                          \
    ++index;                                                                                 \
    auto pRhs = pStack->get(index);                                                          \
                                                                                             \
    pResult = operatorFunction(pLhs, pRhs);                                                  \
                                                                                             \
    LCPP_cont_tailCall(pCont)

namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            Ptr<LispObject> greaterThan(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 1);
                pStack->push(LCPP_pFalse);
                LCPP_cont_tailCall(pCont, &detail::greaterThan_helper);
            }

            Ptr<LispObject> detail::greaterThan_helper(StackPtr<LispObject> pCont)
            {
                LCPP_lambda_builtinFunctions_defineComparisonFunctionHelperContent(number::greaterThan);
            }

            Ptr<LispObject> greaterThanOrEqual(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 1);
                pStack->push(LCPP_pFalse);
                LCPP_cont_tailCall(pCont, &detail::greaterThanOrEqual_helper);
            }

            Ptr<LispObject> detail::greaterThanOrEqual_helper(StackPtr<LispObject> pCont)
            {
                LCPP_lambda_builtinFunctions_defineComparisonFunctionHelperContent(number::greaterThanOrEqual);
            }

            Ptr<LispObject> equal(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 1);
                pStack->push(LCPP_pFalse);
                LCPP_cont_tailCall(pCont, &detail::equal_helper);
            }

            Ptr<LispObject> detail::equal_helper(StackPtr<LispObject> pCont)
            {
                LCPP_lambda_builtinFunctions_defineComparisonFunctionHelperContent(number::equal);
            }

            Ptr<LispObject> lowerThan(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 1);
                pStack->push(LCPP_pFalse);
                LCPP_cont_tailCall(pCont, &detail::lowerThan_helper);
            }

            Ptr<LispObject> detail::lowerThan_helper(StackPtr<LispObject> pCont)
            {
                LCPP_lambda_builtinFunctions_defineComparisonFunctionHelperContent(number::lowerThan);
            }

            Ptr<LispObject> lowerThanOrEqual(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 1);
                pStack->push(LCPP_pFalse);
                LCPP_cont_tailCall(pCont, &detail::lowerThanOrEqual_helper);
            }
            Ptr<LispObject> detail::lowerThanOrEqual_helper(StackPtr<LispObject> pCont)
            {
                LCPP_lambda_builtinFunctions_defineComparisonFunctionHelperContent(number::lowerThanOrEqual);
            }
        }
    }
}

#undef LCPP_lambda_builtinFunctions_defineComparisonFunctionHelperContent
