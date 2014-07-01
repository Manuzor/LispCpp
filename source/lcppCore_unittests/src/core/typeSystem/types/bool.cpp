#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group_SchemeBoolTests("SchemeBoolTests");

    UnitTest g_test_Type(g_group_SchemeBoolTests, "Type", []{
        CUT_ASSERT.isTrue(SCHEME_TRUE.is<SchemeBool>(), "Wrong type declaration for SCHEME_TRUE!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.is<SchemeBool>(), "Wrong type declaration for SCHEME_FALSE!");
    });

    UnitTest g_test_Equality(g_group_SchemeBoolTests, "Equality", []{
        CUT_ASSERT.isTrue(SCHEME_TRUE == SCHEME_TRUE, "Scheme true must equal itself!");

        CUT_ASSERT.isTrue(SCHEME_FALSE == SCHEME_FALSE, "Scheme false must equal itself!");

        CUT_ASSERT.isFalse(SCHEME_TRUE == SCHEME_FALSE, "Scheme true cannot equal scheme false!");
        CUT_ASSERT.isFalse(SCHEME_FALSE == SCHEME_TRUE, "Scheme true cannot equal scheme false!");
    });

    UnitTest g_test_ToString(g_group_SchemeBoolTests, "ToString", []{
        CUT_ASSERT.isTrue(SCHEME_TRUE.toString().IsEqual("#t"), "SCHEME_TRUE has the wrong string representation!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.toString().IsEqual("#f"), "SCHEME_FALSE has the wrong string representation!");
    });

}
