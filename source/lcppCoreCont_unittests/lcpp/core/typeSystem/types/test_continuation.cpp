#include "stdafx.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"

namespace lcpp
{
    static Ptr<LispObject> accumulator(Ptr<LispObject> pCont)
    {
        auto pStack = cont::getStack(pCont);

        auto pToGo = pStack->get(-1);
        auto pCurrentSum = pStack->get(-2);
        pStack->clear();

        auto toGoValue = number::getInteger(pToGo);

        if(toGoValue <= 0)
        {
            LCPP_cont_return(pCont, pCurrentSum);
        }

        auto currentSumValue = number::getInteger(pCurrentSum);

        pStack->push(number::create(currentSumValue + 1));
        pStack->push(number::create(toGoValue - 1));

        LCPP_cont_tailCall(pCont);
    }
}

LCPP_TestGroup(Continuation);

LCPP_TestCase(Continuation, Basics)
{
    auto pContMain = cont::createTopLevel();
    auto pContChild = cont::create(pContMain, &accumulator);

    auto pChildStack = cont::getStack(pContChild);

    auto numIterations = number::Integer_t(3);

    pChildStack->push(number::create(0));
    pChildStack->push(number::create(numIterations));

    cont::trampoline(pContChild);

    auto pResult = cont::getStack(pContMain)->get(-1);

    CUT_ASSERT.isTrue(number::getInteger(pResult) == numIterations);
}
