#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/runtime.h"

#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/string.h"

LCPP_TestGroup(Number);

LCPP_TestCase(Number, Basics)
{
    StackPtr<LispObject> pInteger_One = number::create(1);
    StackPtr<LispObject> pInteger_Two = number::create(2U);
    StackPtr<LispObject> pFloat_One = number::create(1.0);
    StackPtr<LispObject> pFloat_Two = number::create(1.0f);

    CUT_ASSERT.isTrue(object::isType(pInteger_One, Type::Integer));
    CUT_ASSERT.isTrue(object::isType(pInteger_Two, Type::Integer));

    CUT_ASSERT.isTrue(object::isType(pFloat_One, Type::Float));
    CUT_ASSERT.isTrue(object::isType(pFloat_Two, Type::Float));
}

LCPP_TestCase(Number, getInteger)
{
    StackPtr<LispObject> pNumber = number::create(42);
    CUT_ASSERT.isTrue(number::getInteger(pNumber) == 42);
}

LCPP_TestCase(Number, getFloat)
{
    StackPtr<LispObject> pNumber = number::create(3.1415);
    CUT_ASSERT.isTrue(number::getFloat(pNumber) == 3.1415);
}

LCPP_TestCase(Number, toString)
{
    StackPtr<LispObject> pInteger_42 = number::create(42);
    StackPtr<LispObject> pFloat_3_1415 = number::create(3.1415);

    StackPtr<LispObject> pString = LCPP_pNil;

    pString = object::toString(pInteger_42);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("42"));

    pString = object::toString(pFloat_3_1415);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("3.1415"));
}
