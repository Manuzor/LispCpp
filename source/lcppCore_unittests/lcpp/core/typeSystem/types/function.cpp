#include "stdafx.h"
#include "lcpp/testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    int g_testData;

    Ptr<SchemeObject> builtin_testFunc(Ptr<SchemeRuntime> pRuntime,
                                       Ptr<Environment> pEnv,
                                       Ptr<SchemeObject> pArgs)
    {
        g_testData = 1;
        return pRuntime->factory()->createInteger(42);
    }

    UnitTestGroup g_group("SchemeFunctionTest");

    UnitTest g_test1(g_group, "Basics", []{
        auto pRuntime = createTestRuntime();
        auto pEnv = pRuntime->globalEnvironment();

        g_testData = 0;

        auto pFunc = pRuntime->factory()->createBuiltinFunction(
            "testFunc",
            pRuntime->globalEnvironment(),
            &builtin_testFunc);

        CUT_ASSERT.isTrue(g_testData == 0);

        auto pResult = pFunc->call(SCHEME_NIL_PTR);

        CUT_ASSERT.isTrue(g_testData == 1);
        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>());
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 42);
    });
}
