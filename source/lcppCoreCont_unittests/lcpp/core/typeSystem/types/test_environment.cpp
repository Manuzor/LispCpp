#include "stdafx.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"

LCPP_TestGroup(Environment);

LCPP_TestCase(Environment, getName)
{
    auto pEnvParent = env::createTopLevel(symbol::create("the-env"));
    auto pName = env::getName(pEnvParent);

    CUT_ASSERT.isTrue(symbol::getValue(pName).IsEqual("the-env"));
}

LCPP_TestCase(Environment, getParent)
{
    auto pEnvParent = env::createTopLevel(symbol::create("parent"));
    auto pEnvChild = env::create(symbol::create("child"), pEnvParent);

    CUT_ASSERT.isTrue(isNil(env::getParent(pEnvParent)));
    CUT_ASSERT.isTrue(env::getParent(pEnvChild) == pEnvParent);
}

LCPP_TestCase(Environment, getAndAddBinding)
{
    auto pEnvParent = env::createTopLevel(symbol::create("parent"));
    auto pSymbol1 = symbol::create("a");
    auto pInteger = number::create(42);

    auto pKey = Ptr<LispObject>();
    auto result = ezResult(EZ_FAILURE);

    result = env::get(pEnvParent, pSymbol1, pKey);

    CUT_ASSERT.isTrue(result.Failed());

    env::add(pEnvParent, pSymbol1, pInteger);

    result = env::get(pEnvParent, pSymbol1, pKey);

    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pKey == pInteger);

    // Test get in child
    //////////////////////////////////////////////////////////////////////////

    auto pEnvChild = env::create(symbol::create("child"), pEnvParent);

    result = env::get(pEnvChild, pSymbol1, pKey);

    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pKey == pInteger);
}

LCPP_TestCase(Environment, set)
{
    // TODO Implement this test.

    CUT_ASSERT.fail("Not implemented.");
}
