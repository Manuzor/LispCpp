#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("SchemeVoidTests");

    UnitTest g_test1(g_group, "Type", [](){
        CUT_ASSERT.isTrue(SCHEME_VOID.type() == TypeInfo<SchemeVoid>::type(), "SCHEME_VOID.type() returns an incorrect type!");
        CUT_ASSERT.isTrue(SCHEME_VOID.is(TypeInfo<SchemeVoid>::type()), "SCHEME_VOID.is(...) is not working!");
    });

    UnitTest g_test2(g_group, "Equality", [](){
        CUT_ASSERT.isTrue(SCHEME_VOID == SCHEME_VOID, "Scheme nil must equal itself!");
        CUT_ASSERT.isTrue(&SCHEME_VOID == &SCHEME_VOID, "Something is seriously b0rken!");

        CUT_ASSERT.isTrue(SCHEME_VOID != SCHEME_NIL, "Wrong result for equality operator!");
    });

    UnitTest g_test3(g_group, "ToString", [](){
        CUT_ASSERT.isTrue(SCHEME_VOID.toString().IsEqual("#v"), "SCHEME_VOID.toString() should return \"#v\"!");
        CUT_ASSERT.isTrue(SCHEME_VOID.toString().IsEqual("#v"), "Locally constructed SchemeVoid instance v.toString() should return \"#v\"!");
    });
}
