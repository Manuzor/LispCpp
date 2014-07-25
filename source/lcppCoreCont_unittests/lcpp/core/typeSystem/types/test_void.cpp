#include "stdafx.h"
#include "lcpp/core/typeSystem/types/void.h"

LCPP_TEST_GROUP(Void);

LCPP_TEST(Void, Basics)
{
    auto pVoid = void_::create();

    CUT_ASSERT.isTrue(pVoid == LCPP_VOID);
}

LCPP_TEST(Void, isVoid)
{
    auto pVoid = LCPP_VOID;

    CUT_ASSERT.isTrue(isVoid(pVoid));
    CUT_ASSERT.isFalse(isVoid(nullptr));
}
