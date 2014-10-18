#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/exceptions/typeCheckFailedException.h"

#include "lcpp/core/containers/stack.h"

#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/continuation.h"

LCPP_TestGroup(Cons);

LCPP_TestCase(Cons, Basics)
{
    auto pCons = cons::create(LCPP_pNil, LCPP_pNil);

    CUT_ASSERT.isTrue(object::isType(pCons, Type::Cons));
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

LCPP_TestCase(Cons, pushAllReverse)
{
    auto pCons = cons::create(number::create(1),
                              cons::create(number::create(2),
                              cons::create(number::create(3),
                              LCPP_pNil)));
    auto stack = Stack();

    auto count = cons::pushAllReverse(pCons, &stack);

    CUT_ASSERT.isTrue(count == 3);
    CUT_ASSERT.isTrue(number::getInteger(stack.get(0)) == 3);
    CUT_ASSERT.isTrue(number::getInteger(stack.get(1)) == 2);
    CUT_ASSERT.isTrue(number::getInteger(stack.get(2)) == 1);
}

LCPP_TestCase(Cons, toString)
{
    auto pCons = LCPP_pNil;
    auto theString = String();

    pCons = cons::create(LCPP_pNil, LCPP_pNil);
    theString = str::getValue(object::toString(pCons));
    CUT_ASSERT.isTrue(theString.IsEqual("(())"));

    pCons = cons::create(number::create(42), LCPP_pNil);
    theString = str::getValue(object::toString(pCons));
    CUT_ASSERT.isTrue(theString.IsEqual("(42)"));

    pCons = cons::create(number::create(42), number::create(1337));
    theString = str::getValue(object::toString(pCons));
    CUT_ASSERT.isTrue(theString.IsEqual("(42 . 1337)"));

    pCons = cons::create(number::create(42), cons::create(number::create(1337), LCPP_pNil));
    theString = str::getValue(object::toString(pCons));
    CUT_ASSERT.isTrue(theString.IsEqual("(42 1337)"));

    pCons = cons::create(number::create(42), cons::create(number::create(1337), number::create(666)));
    theString = str::getValue(object::toString(pCons));
    CUT_ASSERT.isTrue(theString.IsEqual("(42 1337 . 666)"));

    pCons = cons::create(cons::create(number::create(42), number::create(1337)), LCPP_pNil);
    theString = str::getValue(object::toString(pCons));
    CUT_ASSERT.isTrue(theString.IsEqual("((42 . 1337))"));

    pCons = cons::create(cons::create(number::create(42), LCPP_pNil), number::create(1337));
    theString = str::getValue(object::toString(pCons));
    CUT_ASSERT.isTrue(theString.IsEqual("((42) . 1337)"));
}

LCPP_TestCase(Cons, pack)
{
    StackPtr<LispObject> pCont = cont::createTopLevel(LCPP_test_pRuntimeState);

#define stack (*cont::getStack(pCont))

    stack.push(number::create(1));
    stack.push(number::create(2));
    stack.push(number::create(3));

    StackPtr<LispObject> pCons = LCPP_pNil;
    StackPtr<LispObject> pCar = LCPP_pNil;

    //////////////////////////////////////////////////////////////////////////

    pCons = cons::pack(pCont, 0);

    pCar = cons::getCar(pCons);
    pCons = cons::getCdr(pCons);
    CUT_ASSERT.isTrue(number::getInteger(pCar) == 1);

    pCar = cons::getCar(pCons);
    pCons = cons::getCdr(pCons);
    CUT_ASSERT.isTrue(number::getInteger(pCar) == 2);

    pCar = cons::getCar(pCons);
    pCons = cons::getCdr(pCons);
    CUT_ASSERT.isTrue(number::getInteger(pCar) == 3);
    CUT_ASSERT.isTrue(isNil(pCons));

    //////////////////////////////////////////////////////////////////////////

    pCons = cons::pack(pCont, 0, 2);

    pCar = cons::getCar(pCons);
    pCons = cons::getCdr(pCons);
    CUT_ASSERT.isTrue(number::getInteger(pCar) == 1);

    pCar = cons::getCar(pCons);
    pCons = cons::getCdr(pCons);
    CUT_ASSERT.isTrue(number::getInteger(pCar) == 2);
    CUT_ASSERT.isTrue(isNil(pCons));

    //////////////////////////////////////////////////////////////////////////

    pCons = cons::pack(pCont, -1);

    pCar = cons::getCar(pCons);
    pCons = cons::getCdr(pCons);
    CUT_ASSERT.isTrue(number::getInteger(pCar) == 3);
    CUT_ASSERT.isTrue(isNil(pCons));

    //////////////////////////////////////////////////////////////////////////

    pCons = cons::pack(pCont, -2, 1);

    pCar = cons::getCar(pCons);
    pCons = cons::getCdr(pCons);
    CUT_ASSERT.isTrue(number::getInteger(pCar) == 2);
    CUT_ASSERT.isTrue(isNil(pCons));

#undef stack
}
