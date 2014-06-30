#include "stdafx.h"
#include "testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("EvaluatorTests");

    UnitTest g_test1(g_group, "Recursive_Atoms", []()
    {
        auto pRuntime = createTestRuntime();

        // Integer
        {
            auto test = pRuntime->factory()->createInteger(42);
            auto pResult = pRuntime->evaluator()->evalulate(test);

            CUT_ASSERT.isTrue(pResult->is<SchemeInteger>(), "Wrong evaluation result! Expected integer 42.");
            CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 42, "Expected integer 42.");
            CUT_ASSERT.isTrue(pResult == pRuntime->factory()->createInteger(42), "Double check if instancing failed.");
        }

        // Number
        {
            auto test = pRuntime->factory()->createNumber(3.1415f);
            auto pResult = pRuntime->evaluator()->evalulate(test);

            CUT_ASSERT.isTrue(pResult->is<SchemeNumber>(), "Wrong evaluation result! Expected number 3.1415.");
            CUT_ASSERT.isTrue(pResult.cast<SchemeNumber>()->value() == 3.1415f, "Expected number 3.1415.");
        }

        // String
        {
            auto test = pRuntime->factory()->createString("Hello World");
            auto pResult = pRuntime->evaluator()->evalulate(test);

            CUT_ASSERT.isTrue(pResult->is<SchemeString>(), "Wrong evaluation result! Expected string \"Hello World\".");
            CUT_ASSERT.isTrue(pResult.cast<SchemeString>()->value().IsEqual("Hello World"), "Expected string \"Hello World\".");
        }

        // Symbol
        {
            auto test = pRuntime->factory()->createSymbol("x");

            CUT_ASSERT.throws<lcpp::exceptions::NoBindingFound>([&](){
                auto pResult = pRuntime->evaluator()->evalulate(test);
            });
        }
    });
}
