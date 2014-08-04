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

                auto& pSum = pStack->get(-1);
                auto& pIndex = pStack->get(-2);

                if (number::getInteger(pIndex) >= pStack->size() - 2)
                {
                    LCPP_cont_return(pCont, pSum);
                }

                auto indexValue = number::getInteger(pIndex);

                auto pNext = pStack->get(ezInt32(indexValue));
                pSum = number::create(number::getInteger(pSum) + number::getInteger(pNext));

                pIndex = number::create(number::getInteger(pIndex) + 1);

                LCPP_cont_tailCall(pCont);
            }

            Ptr<LispObject> add(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                // The current index.
                pStack->push(number::create(0));

                // The current sum.
                pStack->push(number::create(0));

                LCPP_cont_tailCall(pCont, &add_helper);
            }
        }
    }
}
