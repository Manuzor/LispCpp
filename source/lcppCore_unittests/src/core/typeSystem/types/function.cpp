#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("SchemeFunctionTest");

    UnitTest g_test1(g_group, "Basics", [](){
        auto env = Environment::createTopLevelInstance();
        TypeFactory factory;
        RecursiveEvaluator eval;

        int testData = 0;

        SchemeFunctionBuiltin func("testFunc", &env, [&](Ptr<Environment>, Ptr<IEvaluator>, Ptr<SchemeObject>) {
            testData = 1;
            return factory.createInteger(42);
        });

        CUT_ASSERT.isTrue(testData == 0);

        auto pResult = func.call(&eval, SCHEME_NIL_PTR);

        CUT_ASSERT.isTrue(testData == 1);
        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>());
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 42);
    });
}
