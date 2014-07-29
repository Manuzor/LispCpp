#include "stdafx.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"

LCPP_TestGroup(Number);

LCPP_TestCase(Number, Basics)
{
    auto pInteger_One = number::create(1);
    auto pInteger_Two = number::create(2U);
    auto pFloat_One = number::create(1.0);
    auto pFloat_Two = number::create(1.0f);

    CUT_ASSERT.isTrue(pInteger_One->isType(Type::Integer));
    CUT_ASSERT.isTrue(pInteger_Two->isType(Type::Integer));

    CUT_ASSERT.isTrue(pFloat_One->isType(Type::Float));
    CUT_ASSERT.isTrue(pFloat_Two->isType(Type::Float));
}

LCPP_TestCase(Number, integerValue)
{
    auto pNumber = number::create(42);
    CUT_ASSERT.isTrue(number::integerValue(pNumber) == 42);
}

LCPP_TestCase(Number, floatValue)
{
    auto pNumber = number::create(3.1415);
    CUT_ASSERT.isTrue(number::floatValue(pNumber) == 3.1415);
}
