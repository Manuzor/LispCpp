#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"

LCPP_TEST_GROUP(Nil);

LCPP_TEST(Nil, Basics)
{
    auto pNil = nil::create();

    CUT_ASSERT.isTrue(pNil == LCPP_NIL);
}

LCPP_TEST(Nil, isNil)
{
    auto pNil = LCPP_NIL;

    CUT_ASSERT.isTrue(isNil(pNil));
    CUT_ASSERT.isFalse(isNil(nullptr));
}
