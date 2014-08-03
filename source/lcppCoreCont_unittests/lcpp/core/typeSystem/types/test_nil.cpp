#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/objectUtils.h"

LCPP_TestGroup(Nil);

LCPP_TestCase(Nil, Basics)
{
    auto pNil = nil::create();

    CUT_ASSERT.isTrue(pNil == LCPP_pNil);
    CUT_ASSERT.isTrue(LCPP_pNil->isType(Type::Nil));
}

LCPP_TestCase(Nil, isNil)
{
    auto pNil = LCPP_pNil;

    CUT_ASSERT.isTrue(isNil(pNil));
    CUT_ASSERT.isFalse(isNil(nullptr));
}

LCPP_TestCase(Nil, toString)
{
    auto pString = toString(LCPP_pNil);

    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("()"));
}
