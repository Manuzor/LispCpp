#include "stdafx.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/containers/stack.h"

LCPP_TestGroup(Symbol);

LCPP_TestCase(Symbol, Basics)
{
    auto pSymbol1 = symbol::create("hello");
}

LCPP_TestCase(Symbol, value)
{
    auto pSymbol = symbol::create("hello-world");

    LCPP_pStack->push(pSymbol);

    CUT_ASSERT.isTrue(LCPP_pStack->size() == 1);
    auto& value = symbol::value(-1);
    CUT_ASSERT.isTrue(LCPP_pStack->size() == 1);

    CUT_ASSERT.isTrue(value.IsEqual("hello-world"));
}

LCPP_TestCase(Symbol, Instancing)
{
    auto pSymbol1 = symbol::create("hello");
    auto pSymbol2 = symbol::create("hello");

    CUT_ASSERT.succeed("Not implemented. Depends on symbol::value()");
}
