#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("SchemeNilTests");

    UnitTest g_test1(g_group, "Type", [](){
        CUT_ASSERT.isTrue(SCHEME_NIL.type() == TypeInfo<SchemeNil>::type(), "SCHEME_NIL.type() returns an incorrect type!");
        CUT_ASSERT.isTrue(SCHEME_NIL.is<SchemeNil>(), "SCHEME_NIL.is(...) is not working!");
        CUT_ASSERT.isTrue(isNil(SCHEME_NIL), "Global function isNil does not work!");
    });

    UnitTest g_test2(g_group, "Equality", [](){
        CUT_ASSERT.isTrue(SCHEME_NIL == SCHEME_NIL, "Scheme nil must equal itself!");
        CUT_ASSERT.isTrue(&SCHEME_NIL == &SCHEME_NIL, "Something is seriously b0rken!");

        CUT_ASSERT.isTrue(SCHEME_NIL != SCHEME_VOID, "Wrong result for equality operator!");

        SchemeObject& nil = SCHEME_NIL;
        CUT_ASSERT.isTrue(nil == SCHEME_NIL);
        CUT_ASSERT.isTrue(&nil == &SCHEME_NIL);
        CUT_ASSERT.isTrue(isNil(nil));
    });

    UnitTest g_test3(g_group, "ToString", [](){
        CUT_ASSERT.isTrue(SCHEME_NIL.toString().IsEqual("()"), "SCHEME_NIL.toString() should return \"()\"!");
        CUT_ASSERT.isTrue(SCHEME_NIL.toString().IsEqual("()"), "Locally constructed SchemeNil instance nil.toString() should return \"()\"!");
    });
}
