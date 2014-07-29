#include "stdafx.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/types/bool.h"

LCPP_TestGroup(Runtime);

auto g_pRuntimeStateCheck = static_cast<void*>(nullptr);

LCPP_TestCase(Runtime, GlobalStateReset1)
{
    if(g_pRuntimeStateCheck == nullptr)
    {
        g_pRuntimeStateCheck = LCPP_pRuntime->syntaxEnvironment().get();
        CUT_ASSERT.succeed("Finished preparing for other unit test.");
        return;
    }

    CUT_ASSERT.isTrue(LCPP_pRuntime->syntaxEnvironment().get() != g_pRuntimeStateCheck);
}

LCPP_TestCase(Runtime, GlobalStateReset2)
{
    if(g_pRuntimeStateCheck == nullptr)
    {
        g_pRuntimeStateCheck = LCPP_pRuntime->syntaxEnvironment().get();
        CUT_ASSERT.succeed("Finished preparing for other unit test.");
        return;
    }

    CUT_ASSERT.isTrue(LCPP_pRuntime->syntaxEnvironment().get() != g_pRuntimeStateCheck);
}

LCPP_TestCase(Runtime, Basics)
{
    auto pRuntime = LCPP_pRuntime;

    CUT_ASSERT.isTrue(pRuntime == LCPP_pRuntime, "Pointers should be the same.");
    CUT_ASSERT.isTrue(pRuntime == LispRuntime::instance());
}
