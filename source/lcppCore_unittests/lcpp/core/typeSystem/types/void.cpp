#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("LispVoidTests");

    UnitTest g_test1(g_group, "Type", []{
        CUT_ASSERT.isTrue(LCPP_VOID->type() == LispVoid::typeInfo(), "SCHEME_VOID.type() returns an incorrect type!");
        CUT_ASSERT.isTrue(LCPP_VOID->is<LispVoid>(), "SCHEME_VOID.is(...) is not working!");
    });

    UnitTest g_test2(g_group, "Equality", []{
        CUT_ASSERT.isTrue(*LCPP_VOID == *LCPP_VOID, "Scheme nil must equal itself!");
        CUT_ASSERT.isTrue(LCPP_VOID == LCPP_VOID, "Something is seriously b0rken!");

        CUT_ASSERT.isTrue(*LCPP_VOID != *LCPP_NIL, "Wrong result for equality operator!");
    });

    UnitTest g_test3(g_group, "ToString", []{
        CUT_ASSERT.isTrue(LCPP_VOID->toString().IsEqual("#v"), "SCHEME_VOID.toString() should return \"#v\"!");
    });
}
