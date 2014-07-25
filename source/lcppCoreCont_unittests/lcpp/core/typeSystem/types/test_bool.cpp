#include "stdafx.h"
#include "lcpp/core/typeSystem/types/bool.h"

LCPP_TEST_GROUP(Bool);

LCPP_TEST(Bool, Basics)
{
    auto pTrue = LCPP_TRUE;
    auto pFalse = LCPP_FALSE;

    CUT_ASSERT.isTrue(pTrue, "Invalid pointer");
    CUT_ASSERT.isTrue(pFalse, "Invalid pointer");

    CUT_ASSERT.isTrue(LCPP_TRUE == true_::create());
    CUT_ASSERT.isTrue(LCPP_FALSE == false_::create());

    CUT_ASSERT.isTrue(pTrue == LCPP_TRUE);
    CUT_ASSERT.isFalse(pFalse == LCPP_TRUE);

    CUT_ASSERT.isTrue(pFalse == LCPP_FALSE);
    CUT_ASSERT.isFalse(pTrue == LCPP_FALSE);
}

LCPP_TEST(Bool, isTrue)
{
    CUT_ASSERT.isTrue(isTrue(LCPP_TRUE));
    CUT_ASSERT.isFalse(isTrue(LCPP_FALSE));
}

LCPP_TEST(Bool, isFalse)
{
    CUT_ASSERT.isTrue(isFalse(LCPP_FALSE));
    CUT_ASSERT.isFalse(isFalse(LCPP_TRUE));
}
