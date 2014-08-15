#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/string.h"

LCPP_TestGroup(Bool);

LCPP_TestCase(Bool, Basics)
{
    auto pTrue = LCPP_pTrue;
    auto pFalse = LCPP_pFalse;

    CUT_ASSERT.isTrue(pTrue, "Invalid pointer");
    CUT_ASSERT.isTrue(pFalse, "Invalid pointer");

    CUT_ASSERT.isTrue(LCPP_pTrue == true_::create());
    CUT_ASSERT.isTrue(LCPP_pFalse == false_::create());

    CUT_ASSERT.isTrue(pTrue == LCPP_pTrue);
    CUT_ASSERT.isFalse(pFalse == LCPP_pTrue);

    CUT_ASSERT.isTrue(pFalse == LCPP_pFalse);
    CUT_ASSERT.isFalse(pTrue == LCPP_pFalse);

    CUT_ASSERT.isTrue(object::isType(pTrue, Type::True));
    CUT_ASSERT.isTrue(object::isType(pFalse, Type::False));
}

LCPP_TestCase(Bool, isTrue)
{
    CUT_ASSERT.isTrue(isTrue(LCPP_pTrue));
    CUT_ASSERT.isFalse(isTrue(LCPP_pFalse));
}

LCPP_TestCase(Bool, isFalse)
{
    CUT_ASSERT.isTrue(isFalse(LCPP_pFalse));
    CUT_ASSERT.isFalse(isFalse(LCPP_pTrue));
}

LCPP_TestCase(Bool, toString)
{
    CUT_ASSERT.isTrue(str::getValue(object::toString(LCPP_pTrue)).IsEqual("#t"));
    CUT_ASSERT.isTrue(str::getValue(object::toString(LCPP_pFalse)).IsEqual("#f"));
}
