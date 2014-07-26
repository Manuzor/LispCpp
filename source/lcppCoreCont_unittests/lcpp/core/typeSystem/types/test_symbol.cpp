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
    CUT_ASSERT.succeed("Not implemented.");
}

LCPP_TestCase(Symbol, Instancing)
{
    auto pSymbol1 = symbol::create("hello");
    auto pSymbol2 = symbol::create("hello");

    CUT_ASSERT.succeed("Not implemented. Depends on symbol::value()");
}
