#include "stdafx.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/string.h"

LCPP_TestGroup(Symbol);

LCPP_TestCase(Symbol, Basics)
{
    auto pSymbol1 = symbol::create("hello");

    CUT_ASSERT.isTrue(object::isType(pSymbol1, Type::Symbol));
}

LCPP_TestCase(Symbol, value)
{
    auto pSymbol = symbol::create("hello-world");

    auto& value = symbol::getValue(pSymbol);

    CUT_ASSERT.isTrue(value.IsEqual("hello-world"));
}

LCPP_TestCase(Symbol, Instancing)
{
    auto pSymbol1 = symbol::create("hello");
    auto pSymbol2 = symbol::create("hello");

    CUT_ASSERT.isTrue(pSymbol1 == pSymbol2);
}

LCPP_TestCase(Symbol, toString)
{
    auto pSymbol = symbol::create("hello");
    auto pString = object::toString(pSymbol);

    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("hello"));
}
