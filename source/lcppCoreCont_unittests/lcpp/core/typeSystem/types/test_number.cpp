#include "stdafx.h"
#include "lcpp/core/typeSystem/types/number.h"

LCPP_TEST_GROUP(Number);

LCPP_TEST(Number, Basics)
{
    auto pOne = number::create(1);

    //CUT_ASSERT.isTrue(number::asInteger(pOne) == 1);
    //CUT_ASSERT.isTrue(number::asFloat(pOne) == 1.0f);
}
