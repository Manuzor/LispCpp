#include "stdafx.h"
#include "lcpp/core/builtins/lambda_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/bool.h"

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"

#define LCPP_lambda_builtinFunctions_defineArithmeticFunctionHelperContent(operatorFunction) \
    typeCheck(pCont, Type::Continuation);                                                    \
                                                                                             \
    auto pStack = cont::getStack(pCont);                                                     \
    auto& index = cont::getUserData(pCont);                                                  \
    const auto maxIndex = pStack->size() - 1;                                                \
                                                                                             \
    auto pCurrentSum = pStack->get(index);                                                   \
                                                                                             \
    if(index >= maxIndex)                                                                    \
    {                                                                                        \
        LCPP_cont_return(pCont, pCurrentSum);                                                \
    }                                                                                        \
                                                                                             \
    ++index;                                                                                 \
                                                                                             \
    auto pLhs = pCurrentSum;                                                                 \
    auto pRhs = pStack->get(index);                                                          \
                                                                                             \
    pCurrentSum = operatorFunction(pLhs, pRhs);                                              \
    pStack->get(index) = pCurrentSum;                                                        \
                                                                                             \
    LCPP_cont_tailCall(pCont)

namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            Ptr<LispObject> add(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 0);

                if(pStack->isEmpty())
                {
                    auto pResult = number::create(0);
                    LCPP_cont_return(pCont, pResult);
                }

                LCPP_cont_tailCall(pCont, &detail::add_helper);
            }

            Ptr<LispObject> detail::add_helper(Ptr<LispObject> pCont)
            {
                LCPP_lambda_builtinFunctions_defineArithmeticFunctionHelperContent(number::add);
            }

            Ptr<LispObject> subtract(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                if (pStack->size() == 1)
                {
                    auto pResult = number::negate(pStack->get(0));
                    LCPP_cont_return(pCont, pResult);
                }

                cont::setUserData(pCont, 0);

                LCPP_cont_tailCall(pCont, &detail::subtract_helper);
            }

            Ptr<LispObject> detail::subtract_helper(Ptr<LispObject> pCont)
            {
                LCPP_lambda_builtinFunctions_defineArithmeticFunctionHelperContent(number::subtract);
            }

            Ptr<LispObject> multiply(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 0);

                if(pStack->isEmpty())
                {
                    auto pResult = number::create(1);
                    LCPP_cont_return(pCont, pResult);
                }

                LCPP_cont_tailCall(pCont, &detail::multiply_helper);
            }

            Ptr<LispObject> detail::multiply_helper(Ptr<LispObject> pCont)
            {
                LCPP_lambda_builtinFunctions_defineArithmeticFunctionHelperContent(number::multiply);
            }

            Ptr<LispObject> divide(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 0);

                auto& pFirstArg = pStack->get(0);

                if(pStack->size() == 1)
                {
                    auto pResult = number::invert(pFirstArg);
                    LCPP_cont_return(pCont, pResult);
                }

                // If the first argument is an integer, make it a float so that the result of the division is always a float as well.
                if(object::isType(pFirstArg, Type::Integer))
                {
                    pFirstArg = number::create(number::Float_t(number::getInteger(pFirstArg)));
                }

                LCPP_cont_tailCall(pCont, &detail::divide_helper);
            }

            Ptr<LispObject> detail::divide_helper(Ptr<LispObject> pCont)
            {
                LCPP_lambda_builtinFunctions_defineArithmeticFunctionHelperContent(number::divide);
            }
        }
    }
}

#undef LCPP_lambda_builtinFunctions_defineArithmeticFunctionHelperContent
