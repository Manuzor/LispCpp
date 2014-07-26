#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"

LCPP_TestGroup(Nil);

LCPP_TestCase(Nil, Basics)
{
    auto pNil = nil::create();

    CUT_ASSERT.isTrue(pNil == LCPP_pNil);
}

LCPP_TestCase(Nil, isNil)
{
    auto pNil = LCPP_pNil;

    CUT_ASSERT.isTrue(isNil(pNil));
    CUT_ASSERT.isFalse(isNil(nullptr));
}
