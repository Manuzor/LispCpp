#include "stdafx.h"
#include "lcpp/core/typeSystem/types/void.h"

LCPP_TestGroup(Void);

LCPP_TestCase(Void, Basics)
{
    auto pVoid = void_::create();

    CUT_ASSERT.isTrue(pVoid == LCPP_pVoid);
}

LCPP_TestCase(Void, isVoid)
{
    auto pVoid = LCPP_pVoid;

    CUT_ASSERT.isTrue(isVoid(pVoid));
    CUT_ASSERT.isFalse(isVoid(nullptr));
}
