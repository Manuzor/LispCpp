#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("SchemeNilTests");

    UnitTest g_test1(g_group, "Type", [](){
        CUT_ASSERT.isTrue(SCHEME_NIL.type() == TypeInfo<SchemeNil>::type(), "SCHEME_NIL.type() returns an incorrect type!");
        CUT_ASSERT.isTrue(SCHEME_NIL.is(TypeInfo<SchemeNil>::type()), "SCHEME_NIL.is(...) is not working!");
    });

    UnitTest g_test2(g_group, "Equality", [](){
        CUT_ASSERT.isTrue(SCHEME_NIL == SCHEME_NIL, "Scheme nil must equal itself!");
        CUT_ASSERT.isTrue(&SCHEME_NIL == &SCHEME_NIL, "Something is seriously b0rken!");

        CUT_ASSERT.isTrue(SCHEME_NIL != SCHEME_VOID, "Wrong result for equality operator!");

        SchemeNil nil;
        CUT_ASSERT.isTrue(nil == SCHEME_NIL, "SCHEME_NIL must equal a locally constructed SchemeNil object!");
        CUT_ASSERT.isTrue(SCHEME_NIL == nil, "SCHEME_NIL must equal a locally constructed SchemeNil object!");
    });

    UnitTest g_test3(g_group, "ToString", [](){
        SchemeNil nil;
        CUT_ASSERT.isTrue(SCHEME_NIL.toString().IsEqual("()"), "SCHEME_NIL.toString() should return \"()\"!");
        CUT_ASSERT.isTrue(SCHEME_NIL.toString().IsEqual("()"), "Locally constructed SchemeNil instance nil.toString() should return \"()\"!");
    });
}
