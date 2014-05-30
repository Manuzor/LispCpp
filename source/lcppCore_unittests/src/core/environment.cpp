#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("EnvironmentTests");

    UnitTest g_test1(g_group, "Basics", [](){
        Environment env = Environment::createTopLevelInstance();
        TypeFactory factory;
        Ptr<SchemeObject> pResult;

        CUT_ASSERT.isFalse(env.get(factory.createSymbol("x"), pResult).IsSuccess(), "'get' returned true for a non-existant key!");
        CUT_ASSERT.isTrue(pResult.isNull(), "'get' altered the out_value even though it failed!");
        env.set(factory.createSymbol("x"), factory.createInteger(42));
        CUT_ASSERT.isTrue(env.get(factory.createSymbol("x"), pResult).IsSuccess(), "'get' returned false for an existing key!");
        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>(), "Wrong type stored in environment!");
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 42, "Wrong type stored in environment!");
    });

}
