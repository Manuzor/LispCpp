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
