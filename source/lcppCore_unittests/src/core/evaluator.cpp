#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("EvaluatorTests");

    UnitTest g_test1(g_group, "Recursive_SimpleEval", []()
    {
        TypeFactory factory;
        RecursiveEvaluator::CInfo cinfo;
        RecursiveEvaluator eval;

        auto& test = factory.createInteger(42);
        auto& result = eval.evalulate(test);

        CUT_ASSERT.isTrue(result.is<SchemeInteger>(), "Wrong evaluation result! Expected integer 42.");
        CUT_ASSERT.isTrue(static_cast<SchemeInteger&>(result).value() == 42, "Expected integer 42.");
        CUT_ASSERT.isTrue(&result == &factory.createInteger(42), "Double check if instancing failed.");
    });
}
