#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"

LCPP_TEST_GROUP(Nil);

LCPP_TEST(Nil, Basics)
{
    auto pNil = LispNil::create();

    CUT_ASSERT.isTrue(pNil == LispNil::create());
}
