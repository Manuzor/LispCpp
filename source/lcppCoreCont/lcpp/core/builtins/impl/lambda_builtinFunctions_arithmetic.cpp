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
    using BinaryLispObjectFunction_t = Ptr<LispObject>(*)(Ptr<LispObject>, Ptr<LispObject>);

    template<BinaryLispObjectFunction_t F>
    struct ArithmeticFunctionHelper
    {
        static Ptr<LispObject> func(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            auto pStack = cont::getStack(pCont);
            auto& index = cont::getUserData(pCont);
            const auto maxIndex = pStack->size() - 1;

            auto pCurrentSum = pStack->get(index);

            if(index >= maxIndex)
            {
                LCPP_cont_return(pCont, pCurrentSum);
            }

            ++index;

            StackPtr<LispObject> pLhs = pCurrentSum;
            StackPtr<LispObject> pRhs = pStack->get(index);

            pCurrentSum = (*F)(pLhs, pRhs);
            cont::getStack(pCont)->get(cont::getUserData(pCont)) = pCurrentSum;

            LCPP_cont_tailCall(pCont);
        }
    };

    namespace lambda
    {
        namespace builtin
        {
            Ptr<LispObject> add(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 1);

                if(pStack->size() == 1)
                {
                    auto pResult = number::create(0);
                    LCPP_cont_return(pCont, pResult);
                }

                LCPP_cont_tailCall(pCont, &ArithmeticFunctionHelper<&number::add>::func);
            }

            Ptr<LispObject> subtract(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                if (pStack->size() == 2)
                {
                    auto pResult = number::negate(pStack->get(1));
                    LCPP_cont_return(pCont, pResult);
                }

                cont::setUserData(pCont, 1);

                LCPP_cont_tailCall(pCont, &ArithmeticFunctionHelper<&number::subtract>::func);
            }

            Ptr<LispObject> multiply(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 1);

                if(pStack->size() == 1)
                {
                    auto pResult = number::create(1);
                    LCPP_cont_return(pCont, pResult);
                }

                LCPP_cont_tailCall(pCont, &ArithmeticFunctionHelper<&number::multiply>::func);
            }

            Ptr<LispObject> divide(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                cont::setUserData(pCont, 1);

                auto& pFirstArg = pStack->get(1);

                if(pStack->size() == 2)
                {
                    auto pResult = number::invert(pFirstArg);
                    LCPP_cont_return(pCont, pResult);
                }

                // If the first argument is an integer, make it a float so that the result of the division is always a float as well.
                if(object::isType(pFirstArg, Type::Integer))
                {
                    pFirstArg = number::create(number::Float_t(number::getInteger(pFirstArg)));
                }

                LCPP_cont_tailCall(pCont, &ArithmeticFunctionHelper<&number::divide>::func);
            }

            Ptr<LispObject> modulo(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pLhs = pStack->get(1);
                auto pRhs = pStack->get(2);

                LCPP_cont_return(pCont, number::modulo(pLhs, pRhs));
            }
        }
    }
}

#undef LCPP_lambda_builtinFunctions_defineArithmeticFunctionHelperContent
