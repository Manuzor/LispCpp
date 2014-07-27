#include "stdafx.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/number.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/typeCheck.h"

LCPP_TestGroup(Environment);

LCPP_TestCase(Environment, Basics)
{
    auto pEnv = env::createTopLevel(symbol::create("top-level-env"));

    auto pName = pEnv->getBody().m_env.m_pName;

    CUT_ASSERT.throwsNothing([&]{ typeCheck(pName, Type::Symbol); });

    CUT_ASSERT.isTrue(pEnv);
}

LCPP_TestCase(Environment, add)
{
    auto pEnv = env::createTopLevel(symbol::create("test-env"));
    auto pSymbol = symbol::create("key-symbol");
    auto pInteger = number::create(42);

    LCPP_pStack->push(pEnv);
    LCPP_pStack->push(pSymbol);
    LCPP_pStack->push(pInteger);

    CUT_ASSERT.isTrue(LCPP_pStack->size() == 3);

    env::add();

    CUT_ASSERT.isTrue(LCPP_pStack->isEmpty());

    CUT_ASSERT.succeed("Not fully implemented.");
}

LCPP_TestCase(Environment, get)
{
    CUT_ASSERT.succeed("Not implemented.");
}

LCPP_TestCase(Environment, set)
{
    CUT_ASSERT.succeed("Not implemented.");
}
