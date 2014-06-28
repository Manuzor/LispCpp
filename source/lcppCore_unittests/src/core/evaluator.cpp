#include "stdafx.h"
#include "testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("EvaluatorTests");

    UnitTest g_test1(g_group, "Recursive_SimpleEval", []()
    {
        auto pRuntime = createTestRuntime();

        auto& test = pRuntime->factory()->createInteger(42);
        auto pResult = pRuntime->evaluator()->evalulate(test);

        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>(), "Wrong evaluation result! Expected integer 42.");
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 42, "Expected integer 42.");
        CUT_ASSERT.isTrue(pResult == pRuntime->factory()->createInteger(42), "Double check if instancing failed.");
    });
}
