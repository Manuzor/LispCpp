#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group_LispBoolTests("LispBoolTests");

    UnitTest g_test_Type(g_group_LispBoolTests, "Type", []{
        CUT_ASSERT.isTrue(LCPP_TRUE->is<LispBool>(), "Wrong type declaration for LCPP_TRUE!");
        CUT_ASSERT.isTrue(LCPP_FALSE->is<LispBool>(), "Wrong type declaration for LCPP_FALSE!");
    });

    UnitTest g_test_Equality(g_group_LispBoolTests, "Equality", []{
        CUT_ASSERT.isTrue(*LCPP_TRUE == *LCPP_TRUE, "Scheme true must equal itself!");

        CUT_ASSERT.isTrue(*LCPP_FALSE == *LCPP_FALSE, "Scheme false must equal itself!");

        CUT_ASSERT.isFalse(*LCPP_TRUE == *LCPP_FALSE, "Scheme true cannot equal scheme false!");
        CUT_ASSERT.isFalse(*LCPP_FALSE == *LCPP_TRUE, "Scheme true cannot equal scheme false!");
    });

    UnitTest g_test_ToString(g_group_LispBoolTests, "ToString", []{
        CUT_ASSERT.isTrue(LCPP_TRUE->toString().IsEqual("#t"), "LCPP_TRUE has the wrong string representation!");
        CUT_ASSERT.isTrue(LCPP_FALSE->toString().IsEqual("#f"), "LCPP_FALSE has the wrong string representation!");
    });

    UnitTest g_test_Value(g_group_LispBoolTests, "Value", []{
        CUT_ASSERT.isTrue(LCPP_TRUE->value(), "LCPP_TRUE has the wrong value!");
        CUT_ASSERT.isFalse(LCPP_FALSE->value(), "LCPP_FALSE has the wrong value!");
    });

}
