#include "stdafx.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/containers/stack.h"

LCPP_TEST_GROUP(Runtime);

LCPP_TEST(Runtime, Basics)
{
    auto pRuntime = LCPP_pRuntime;

    CUT_ASSERT.isTrue(pRuntime == LCPP_pRuntime, "Pointers should be the same.");
    CUT_ASSERT.isTrue(pRuntime == LispRuntime::instance());
}

LCPP_TEST(Runtime, Stack)
{
    auto pStack = LCPP_pRuntime->getStack();

    CUT_ASSERT.isTrue(pStack == LCPP_pStack);
}
