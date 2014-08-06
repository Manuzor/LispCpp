#include "stdafx.h"
#include "lcpp/core/builtins/syntax_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/number.h"

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"


namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            static Ptr<LispObject> add_helper(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                auto& index = cont::getUserData(pCont);

                auto& pSum = pStack->get(-1);

                if(index >= pStack->size() - 1)
                {
                    LCPP_cont_return(pCont, pSum);
                }

                auto pNext = pStack->get(ezInt32(index));
                auto lhs = number::getInteger(pSum);
                auto rhs = number::getInteger(pNext);
                pSum = number::create(lhs + rhs);

                ++index;

                LCPP_cont_tailCall(pCont);
            }

            Ptr<LispObject> add(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                auto& index = cont::getUserData(pCont);
                index = 0;

                // The current sum.
                pStack->push(number::create(0));

                LCPP_cont_tailCall(pCont, &add_helper);
            }
        }
    }
}
