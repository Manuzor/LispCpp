#include "stdafx.h"
#include "lcpp/testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    int g_testData;

    Ptr<LispObject> builtin_testFunc(Ptr<LispRuntime> pRuntime,
                                       Ptr<Environment> pEnv,
                                       Ptr<LispObject> pArgs)
    {
        g_testData = 1;
        return pRuntime->factory()->createInteger(42);
    }

    UnitTestGroup g_group("LispFunctionTest");

    UnitTest g_test1(g_group, "Basics", []{
        auto pRuntime = createTestRuntime();
        auto pEnv = pRuntime->globalEnvironment();

        g_testData = 0;

        auto pFunc = pRuntime->factory()->createBuiltinFunction(
            "testFunc",
            pRuntime->globalEnvironment(),
            &builtin_testFunc);

        CUT_ASSERT.isTrue(g_testData == 0);

        auto pResult = pFunc->call(LCPP_NIL);

        CUT_ASSERT.isTrue(g_testData == 1);
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 42);
    });
}
