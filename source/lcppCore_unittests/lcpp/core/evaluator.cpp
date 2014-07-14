#include "stdafx.h"
#include "lcpp/testRuntime.h"

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
            auto test = LispInteger::create(42);
            auto pResult = pRuntime->evaluator()->evalulate(test);

            CUT_ASSERT.isTrue(pResult->is<LispInteger>(), "Wrong evaluation result! Expected integer 42.");
            CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 42, "Expected integer 42.");
            CUT_ASSERT.isTrue(pResult == LispInteger::create(42), "Double check if instancing failed.");
        }

        // Number
        {
            auto test = LispNumber::create(3.1415f);
            auto pResult = pRuntime->evaluator()->evalulate(test);

            CUT_ASSERT.isTrue(pResult->is<LispNumber>(), "Wrong evaluation result! Expected number 3.1415.");
            CUT_ASSERT.isTrue(pResult.cast<LispNumber>()->value() == 3.1415f, "Expected number 3.1415.");
        }

        // String
        {
            auto test = LispString::create("Hello World");
            auto pResult = pRuntime->evaluator()->evalulate(test);

            CUT_ASSERT.isTrue(pResult->is<LispString>(), "Wrong evaluation result! Expected string \"Hello World\".");
            CUT_ASSERT.isTrue(pResult.cast<LispString>()->value().IsEqual("Hello World"), "Expected string \"Hello World\".");
        }

        // Symbol
        {
            auto test = LispSymbol::create("x");

            CUT_ASSERT.throws<lcpp::exceptions::NoBindingFound>([&]{
                auto pResult = pRuntime->evaluator()->evalulate(test);
            });
        }
    });

    UnitTest g_test2(g_group, "Recursive_SymbolLookup", []{
        auto pRuntime = createTestRuntime();

        auto pSymbol = LispSymbol::create("x");

        // Not found
        {
            CUT_ASSERT.throws<lcpp::exceptions::NoBindingFound>([&]{
                auto pResult = pRuntime->evaluator()->evalulate(pSymbol);
            });
        }

        // Define x as integer 1337 in global env
        pRuntime->globalEnvironment()->add(pSymbol, LispInteger::create(1337));

        // Lookup symbol x
        {
            Ptr<LispObject> pResult;
            pResult = pRuntime->evaluator()->evalulate(pSymbol);

            CUT_ASSERT.isTrue(pResult->is<LispInteger>(), "Expected result type to be integer!");
            CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 1337, cut::format("Expected integer value to be 1337, got %d.", pResult.cast<LispInteger>()->value()));
        }

        // define f as a built-in function
        pSymbol = LispSymbol::create("f");
        auto pLambda = pRuntime->factory()->createBuiltinFunction("test-func", pRuntime->globalEnvironment(),
            [](Ptr<Environment> pEnv, Ptr<LispObject> pArgs) -> Ptr<LispObject>
        {
            return LispInteger::create(42);
        });
        pRuntime->globalEnvironment()->add(pSymbol, pLambda);

        {
            auto pResult = pRuntime->evaluator()->evalulate(pSymbol);

            CUT_ASSERT.isTrue(pResult->is<LispFunction>());

            pResult = pRuntime->evaluator()->evalulate(LispCons::create(pResult, LCPP_NIL));

            CUT_ASSERT.isTrue(pResult->is<LispInteger>(), "Expected the return value of the function to be an integer!");
            CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 42, "Expected the integer 42!");
        }
    });

    UnitTest g_test3(g_group, "Recursive_Clojures", []{
        auto pRuntime = createTestRuntime();

        auto pSymbol = LispSymbol::create("x");

        // define x as 123
        pRuntime->globalEnvironment()->add(pSymbol, LispInteger::create(123));

        auto pLambda = pRuntime->factory()->createBuiltinFunction("test-clojure", pRuntime->globalEnvironment(),
            [](Ptr<Environment> pEnv, Ptr<LispObject> pArgs) -> Ptr<LispObject>
        {
            Ptr<LispObject> pResult;
            auto lookupResult = pEnv->get(LispSymbol::create("x"), pResult);
            CUT_ASSERT.isTrue(lookupResult.IsSuccess(), "Failed to find 'x' in environment!");
            return pResult;
        });

        auto pFunctionCallObject = LispCons::create(pLambda, LCPP_NIL);

        auto pResult = pRuntime->evaluator()->evalulate(pFunctionCallObject);

        CUT_ASSERT.isTrue(pResult->is<LispInteger>(), "Failed lookup of 'x' in lambda!");
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 123, "Wrong value for 'x'!");
    });
}
