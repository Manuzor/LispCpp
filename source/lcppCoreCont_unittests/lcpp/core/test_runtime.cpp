#include "stdafx.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/types/bool.h"

LCPP_TestGroup(Runtime);

LCPP_TestCase(Runtime, GlobalStateReset1)
{
    CUT_ASSERT.isTrue(LCPP_pStack->isEmpty());
    LCPP_pStack->push(LCPP_pTrue);
    CUT_ASSERT.isFalse(LCPP_pStack->isEmpty());
    CUT_ASSERT.isTrue(LCPP_pStack->get(-1) == LCPP_pTrue);
}

LCPP_TestCase(Runtime, GlobalStateReset2)
{
    CUT_ASSERT.isTrue(LCPP_pStack->isEmpty());
    LCPP_pStack->push(LCPP_pFalse);
    CUT_ASSERT.isFalse(LCPP_pStack->isEmpty());
    CUT_ASSERT.isTrue(LCPP_pStack->get(-1) == LCPP_pFalse);
}

LCPP_TestCase(Runtime, Basics)
{
    auto pRuntime = LCPP_pRuntime;

    CUT_ASSERT.isTrue(pRuntime == LCPP_pRuntime, "Pointers should be the same.");
    CUT_ASSERT.isTrue(pRuntime == LispRuntime::instance());
}

LCPP_TestCase(Runtime, Stack)
{
    auto pStack = LCPP_pRuntime->getStack();

    CUT_ASSERT.isTrue(pStack == LCPP_pStack);
}
