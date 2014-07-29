#include "stdafx.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"

LCPP_TestGroup(Environment);

LCPP_TestCase(Environment, Basics)
{
    auto pEnv = env::createTopLevel(symbol::create("top-level-env"));

    //CUT_ASSERT.throwsNothing([&]{ typeCheck(env::name(pEnv), Type::Symbol); });
    //CUT_ASSERT.isTrue(isNil(env::parent(pEnv)));

    CUT_ASSERT.isTrue(pEnv);
}

LCPP_TestCase(Environment, add)
{
    auto pEnv = env::createTopLevel(symbol::create("test-env"));
    auto pSymbol = symbol::create("key-symbol");
    auto pInteger = number::create(42);

    // TODO Implement this test.

    CUT_ASSERT.fail("Not implemented.");
}

LCPP_TestCase(Environment, get)
{
    // TODO Implement this test.

    CUT_ASSERT.fail("Not implemented.");
}

LCPP_TestCase(Environment, set)
{
    // TODO Implement this test.

    CUT_ASSERT.fail("Not implemented.");
}
