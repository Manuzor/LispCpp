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

            CUT_ASSERT.throws<lcpp::exceptions::NoBindingFound>([&]{
                auto pResult = pRuntime->evaluator()->evalulate(test);
            });
        }
    });

    UnitTest g_test2(g_group, "Recursive_SymbolLookup", []{
        auto pRuntime = createTestRuntime();

        auto pSymbol = pRuntime->factory()->createSymbol("x");

        // Not found
        {
            CUT_ASSERT.throws<lcpp::exceptions::NoBindingFound>([&]{
                auto pResult = pRuntime->evaluator()->evalulate(pSymbol);
            });
        }

        // Define x as integer 1337 in global env
        pRuntime->globalEnvironment()->add(pSymbol, pRuntime->factory()->createInteger(1337));

        // Lookup symbol x
        {
            Ptr<SchemeObject> pResult;
            pResult = pRuntime->evaluator()->evalulate(pSymbol);

            CUT_ASSERT.isTrue(pResult->is<SchemeInteger>(), "Expected result type to be integer!");
            CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 1337, cut::format("Expected integer value to be 1337, got %d.", pResult.cast<SchemeInteger>()->value()));
        }

        // define f as a built-in function
        pSymbol = pRuntime->factory()->createSymbol("f");
        auto pLambda = pRuntime->factory()->createBuiltinFunction("test-func", pRuntime->globalEnvironment(),
            [](Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs) -> Ptr<SchemeObject>
        {
            return pRuntime->factory()->createInteger(42);
        });
        pRuntime->globalEnvironment()->add(pSymbol, pLambda);

        {
            auto pResult = pRuntime->evaluator()->evalulate(pSymbol);

            CUT_ASSERT.isTrue(pResult->is<SchemeFunction>());

            pResult = pRuntime->evaluator()->evalulate(pRuntime->factory()->createCons(pResult, SCHEME_NIL_PTR));

            CUT_ASSERT.isTrue(pResult->is<SchemeInteger>(), "Expected the return value of the function to be an integer!");
            CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 42, "Expected the integer 42!");
        }
    });

    UnitTest g_test3(g_group, "Recursive_Clojures", []{
        auto pRuntime = createTestRuntime();

        auto pSymbol = pRuntime->factory()->createSymbol("x");

        // define x as 123
        pRuntime->globalEnvironment()->add(pSymbol, pRuntime->factory()->createInteger(123));

        auto pLambda = pRuntime->factory()->createBuiltinFunction("test-clojure", pRuntime->globalEnvironment(),
                                                                  [](Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs) -> Ptr<SchemeObject>
        {
            Ptr<SchemeObject> pResult;
            auto lookupResult = pEnv->get(pRuntime->factory()->createSymbol("x"), pResult);
            CUT_ASSERT.isTrue(lookupResult.IsSuccess(), "Failed to find 'x' in environment!");
            return pResult;
        });

        auto pFunctionCallObject = pRuntime->factory()->createCons(pLambda, SCHEME_NIL_PTR);

        auto pResult = pRuntime->evaluator()->evalulate(pFunctionCallObject);

        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>(), "Failed lookup of 'x' in lambda!");
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 123, "Wrong value for 'x'!");
    });
}
