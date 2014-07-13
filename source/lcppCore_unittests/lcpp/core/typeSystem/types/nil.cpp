#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("LispNilTests");

    UnitTest g_test1(g_group, "Type", []{
        CUT_ASSERT.isTrue(LCPP_NIL->type() == LispNil::typeInfo(), "SCHEME_NIL.type() returns an incorrect type!");
        CUT_ASSERT.isTrue(LCPP_NIL->is<LispNil>(), "SCHEME_NIL.is(...) is not working!");
        CUT_ASSERT.isTrue(isNil(LCPP_NIL), "Global function isNil does not work!");
    });

    UnitTest g_test2(g_group, "Equality", []{
        CUT_ASSERT.isTrue(*LCPP_NIL == *LCPP_NIL, "Scheme nil must equal itself!");
        CUT_ASSERT.isTrue(LCPP_NIL == LCPP_NIL, "Something is seriously b0rken!");

        CUT_ASSERT.isTrue(*LCPP_NIL != *LCPP_VOID, "Wrong result for equality operator!");

        CUT_ASSERT.isTrue(isNil(LCPP_NIL));
    });

    UnitTest g_test3(g_group, "ToString", []{
        CUT_ASSERT.isTrue(LCPP_NIL->toString().IsEqual("()"), "LispNil::toString() should return \"()\"!");
    });
}
