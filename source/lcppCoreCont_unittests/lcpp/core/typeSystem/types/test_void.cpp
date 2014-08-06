#include "stdafx.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/string.h"

LCPP_TestGroup(Void);

LCPP_TestCase(Void, Basics)
{
    auto pVoid = void_::create();

    CUT_ASSERT.isTrue(pVoid == LCPP_pVoid);
    CUT_ASSERT.isTrue(object::isType(pVoid, Type::Void));
}

LCPP_TestCase(Void, isVoid)
{
    auto pVoid = LCPP_pVoid;

    CUT_ASSERT.isTrue(isVoid(pVoid));
    CUT_ASSERT.isFalse(isVoid(nullptr));
}

LCPP_TestCase(Void, toString)
{
    auto pString = object::toString(LCPP_pVoid);

    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("#v"));
}
