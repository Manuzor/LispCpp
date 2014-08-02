#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/exceptions/typeCheckFailedException.h"

LCPP_TestGroup(Cons);

LCPP_TestCase(Cons, Basics)
{
    auto pCons = cons::create(LCPP_pNil, LCPP_pNil);

    CUT_ASSERT.isTrue(pCons->isType(Type::Cons));
    CUT_ASSERT.isTrue(isNil(cons::getCar(pCons)));
    CUT_ASSERT.isTrue(isNil(cons::getCdr(pCons)));
}

LCPP_TestCase(Cons, CarCdrGettersTypeCheck)
{
    CUT_ASSERT.throws<exceptions::TypeCheckFailed>([]{ cons::getCar(LCPP_pNil); });
    CUT_ASSERT.throws<exceptions::TypeCheckFailed>([]{ cons::getCdr(LCPP_pNil); });
}

LCPP_TestCase(Cons, pushAll)
{
    auto pCons = cons::create(number::create(1),
                 cons::create(number::create(2),
                 cons::create(number::create(3),
                 LCPP_pNil)));
    auto stack = Stack();

    auto count = cons::pushAll(pCons, &stack);

    CUT_ASSERT.isTrue(count == 3);
    CUT_ASSERT.isTrue(number::getInteger(stack.get(0)) == 1);
    CUT_ASSERT.isTrue(number::getInteger(stack.get(1)) == 2);
    CUT_ASSERT.isTrue(number::getInteger(stack.get(2)) == 3);
}
