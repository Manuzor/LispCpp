#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/runtime.h"

#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    static Ptr<LispObject> accumulator(StackPtr<LispObject> pCont)
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
    StackPtr<LispObject> pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    CUT_ASSERT.isTrue(object::isType(pContMain, Type::Continuation));

    StackPtr<LispObject> pContChild = cont::create(pContMain, &accumulator);

    auto numIterations = number::Integer_t(3);

    cont::getStack(pContChild)->push(number::create(0));
    cont::getStack(pContChild)->push(number::create(numIterations));

    cont::trampoline(pContChild);

    auto pResult = cont::getStack(pContMain)->get(-1);

    CUT_ASSERT.isTrue(number::getInteger(pResult) == numIterations);
}

LCPP_TestCase(Continuation, toString)
{
    auto dummyFunc = [](StackPtr<LispObject>){ return Ptr<LispObject>(LCPP_pNil); };
    StackPtr<LispObject> pContRoot = cont::createTopLevel(LCPP_test_pRuntimeState);
    StackPtr<LispObject> pContChild = cont::create(pContRoot, dummyFunc);

    StackPtr<LispObject> pString = LCPP_pNil;

    pString = object::toString(pContRoot);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<continuation>"));

    pString = object::toString(pContChild);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<continuation>"));
}
