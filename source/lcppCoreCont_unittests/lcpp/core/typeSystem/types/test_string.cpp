#include "stdafx.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectUtils.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"

LCPP_TestGroup(String);

LCPP_TestCase(String, Basics)
{
    auto pString = str::create("hello");

    CUT_ASSERT.isTrue(pString->isType(Type::String));
}

LCPP_TestCase(String, value)
{
    auto pString = str::create("hello-world");

    auto& value = str::getValue(pString);

    CUT_ASSERT.isTrue(value.IsEqual("hello-world"));
}

LCPP_TestCase(String, toString)
{
    auto pString = str::create("hello world");

    pString = toString(pString);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("\"hello world\""));
}
