#include "stdafx.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/containers/stack.h"

LCPP_TestGroup(Runtime);

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
