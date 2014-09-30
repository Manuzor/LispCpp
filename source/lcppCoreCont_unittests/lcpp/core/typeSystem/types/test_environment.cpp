#include "stdafx.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/types/nil.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/string.h"

LCPP_TestGroup(Environment);

LCPP_TestCase(Environment, Basics)
{
    auto pEnv = env::createTopLevel(symbol::create("env"));

    CUT_ASSERT.isTrue(object::isType(pEnv, Type::Environment));
}

LCPP_TestCase(Environment, getName)
{
    auto pEnvParent = env::createTopLevel(symbol::create("the-env"));
    auto pName = env::getName(pEnvParent);

    CUT_ASSERT.isTrue(symbol::getValue(pName).IsEqual("the-env"));

    auto pEnvChild = env::create(pEnvParent, symbol::create("the-child"));
    pName = env::getName(pEnvChild);

    CUT_ASSERT.isTrue(symbol::getValue(pName).IsEqual("the-child"));
}

LCPP_TestCase(Environment, getParent)
{
    auto pEnvParent = env::createTopLevel(symbol::create("parent"));
    auto pEnvChild = env::create(pEnvParent, symbol::create("child"));

    CUT_ASSERT.isTrue(isNil(env::getParent(pEnvParent)));
    CUT_ASSERT.isTrue(env::getParent(pEnvChild) == pEnvParent);
}

LCPP_TestCase(Environment, getBinding_addBinding_setBinding)
{
    auto pEnvParent = env::createTopLevel(symbol::create("parent"));
    auto pSymbol_a = symbol::create("a");
    auto pSymbol_b = symbol::create("b");
    auto pInteger_42 = number::create(42);
    auto pInteger_1337 = number::create(1337);

    auto pResultObject = StackPtr<LispObject>();
    auto result = ezResult(EZ_FAILURE);

    result = env::getBinding(pEnvParent, pSymbol_a, pResultObject);

    CUT_ASSERT.isTrue(result.Failed());
    env::addBinding(pEnvParent, pSymbol_a, pInteger_42); ///< parent => a: 42

    result = env::getBinding(pEnvParent, pSymbol_a, pResultObject);
    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pResultObject == pInteger_42);

    result = env::setBinding(pEnvParent, pSymbol_b, pInteger_1337);
    CUT_ASSERT.isTrue(result.Failed(), "Should not be able to set a binding for a non-existant symbol.");

    result = env::setBinding(pEnvParent, pSymbol_a, pInteger_1337); ///< parent => a: 1337
    CUT_ASSERT.isTrue(result.Succeeded());

    result = env::getBinding(pEnvParent, pSymbol_a, pResultObject);
    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pResultObject == pInteger_1337);

    // parent/child
    //////////////////////////////////////////////////////////////////////////

    auto pEnvChild = env::create(pEnvParent, symbol::create("child"));

    result = env::getBinding(pEnvChild, pSymbol_a, pResultObject);
    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pResultObject == pInteger_1337);

    // set non-existant (should fail)
    result = env::setBinding(pEnvChild, pSymbol_b, pInteger_1337);
    CUT_ASSERT.isTrue(result.Failed());

    // set in parent via child
    result = env::setBinding(pEnvChild, pSymbol_a, pInteger_42); ///< parent => a: 42
    CUT_ASSERT.isTrue(result.Succeeded());

    // Check that the child set symbol "a" in the parent.
    result = env::getBinding(pEnvParent, pSymbol_a, pResultObject);
    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pResultObject == pInteger_42);

    // Add in child / no effect on parent
    //////////////////////////////////////////////////////////////////////////

    env::addBinding(pEnvChild, pSymbol_a, pInteger_1337); ///< parent => a: 42 | parent/child => a: 1337

    result = env::getBinding(pEnvParent, pSymbol_a, pResultObject);
    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pResultObject == pInteger_42);

    result = env::getBinding(pEnvChild, pSymbol_a, pResultObject);
    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pResultObject == pInteger_1337);

    result = env::setBinding(pEnvChild, pSymbol_a, pInteger_42); ///< parent => a: 42 | parent/child => a: 42
    CUT_ASSERT.isTrue(result.Succeeded());

    result = env::getBinding(pEnvChild, pSymbol_a, pResultObject);
    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pResultObject == pInteger_42);
    
    // Add in child and show that it does not affect the parent.
    env::addBinding(pEnvChild, pSymbol_b, pInteger_1337);

    result = env::getBinding(pEnvChild, pSymbol_b, pResultObject);
    CUT_ASSERT.isTrue(result.Succeeded());
    CUT_ASSERT.isTrue(pResultObject == pInteger_1337);

    result = env::getBinding(pEnvParent, pSymbol_b, pResultObject);
    CUT_ASSERT.isTrue(result.Failed());
}

LCPP_TestCase(Environment, qualifiedName)
{
    auto pEnvParent = env::createTopLevel(symbol::create("parent"));     ///< parent
    auto pEnvChild1 = env::create(pEnvParent, symbol::create("child1")); ///< parent/child1
    auto pEnvChild2 = env::create(pEnvChild1, symbol::create("child2")); ///< parent/child1/child2
    auto pEnvChild3 = env::create(pEnvChild2, symbol::create("child3")); ///< parent/child1/child2/child3

    auto pName = StackPtr<LispObject>();

    //////////////////////////////////////////////////////////////////////////

    pName = env::getQualifiedName(pEnvParent);
    CUT_ASSERT.isTrue(str::getValue(pName).IsEqual("parent"));

    pName = env::getQualifiedName(pEnvChild1);
    CUT_ASSERT.isTrue(str::getValue(pName).IsEqual("parent/child1"));

    pName = env::getQualifiedName(pEnvChild2);
    CUT_ASSERT.isTrue(str::getValue(pName).IsEqual("parent/child1/child2"));

    pName = env::getQualifiedName(pEnvChild3);
    CUT_ASSERT.isTrue(str::getValue(pName).IsEqual("parent/child1/child2/child3"));
}

LCPP_TestCase(Environment, toString)
{
    // Should return the same thing as getQualifiedName.

    auto pEnv = env::createTopLevel(symbol::create("the-env"));

    auto pStringLhs = env::toString(pEnv);
    auto pStringRhs = env::getQualifiedName(pEnv);

    CUT_ASSERT.isTrue(str::getValue(pStringLhs).IsEqual(str::getValue(pStringRhs).GetData()));
}

LCPP_TestCase(Environment, existsBinding)
{
    auto pEnvParent = env::createTopLevel(symbol::create("parent"));
    auto pEnvChild = env::create(pEnvParent, symbol::create("child"));
    auto pSymbol_a = symbol::create("a");
    auto pSymbol_b = symbol::create("b");
    auto pInteger_42 = number::create(42);
    auto pInteger_1337 = number::create(1337);

    auto result = env::BindingLocation(env::BindingLocation::None);

    result = env::existsBinding(pEnvParent, pSymbol_a);
    CUT_ASSERT.isFalse(result);

    env::addBinding(pEnvParent, pSymbol_a, pInteger_42);

    result = env::existsBinding(pEnvParent, pSymbol_a);
    CUT_ASSERT.isTrue(result);
    CUT_ASSERT.isTrue(result.locally());

    result = env::existsBinding(pEnvChild, pSymbol_a);
    CUT_ASSERT.isTrue(result);
    CUT_ASSERT.isTrue(result.inParent());

    env::addBinding(pEnvChild, pSymbol_b, pInteger_1337);

    result = env::existsBinding(pEnvParent, pSymbol_b);
    CUT_ASSERT.isFalse(result);

    result = env::existsBinding(pEnvChild, pSymbol_b);
    CUT_ASSERT.isTrue(result);
    CUT_ASSERT.isTrue(result.locally());
}
