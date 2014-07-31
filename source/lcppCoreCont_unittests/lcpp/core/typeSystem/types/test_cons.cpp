#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"

LCPP_TestGroup(Cons);

LCPP_TestCase(Cons, Basics)
{
    auto pCons = cons::create(LCPP_pNil, LCPP_pNil);

    CUT_ASSERT.isTrue(isNil(cons::getCar(pCons)));
    CUT_ASSERT.isTrue(isNil(cons::getCdr(pCons)));
}
