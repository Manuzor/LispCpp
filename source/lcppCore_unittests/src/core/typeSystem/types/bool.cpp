#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group_SchemeBoolTests("SchemeBoolTests");

    UnitTest g_test_Type(g_group_SchemeBoolTests, "Type", [](){
        CUT_ASSERT.isTrue(SCHEME_TRUE.is(TypeInfo<SchemeBool>::type()), "Wrong type declaration for SCHEME_TRUE!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.is(TypeInfo<SchemeBool>::type()), "Wrong type declaration for SCHEME_FALSE!");
    });

    UnitTest g_test_Construction(g_group_SchemeBoolTests, "Construction", [](){
        SchemeBool t(true);
        SchemeBool f(false);

        SchemeBool tOther(t);
        SchemeBool fOther = false;

        CUT_ASSERT.isTrue( t.value(),      "SchemeBool(true) appears to be not working correctly.");
        CUT_ASSERT.isFalse(f.value(),      "SchemeBool(false) appears to be not working correctly.");
        CUT_ASSERT.isTrue( tOther.value(), "Copy constructor of SchemeBool failed to construct object with the correct value.");

        auto implicitConversion = [](SchemeBool b) -> bool
        {
            CUT_ASSERT.isTrue(b.value(), "Implicit conversion failed."); return b;
        };

        CUT_ASSERT.isTrue(implicitConversion(true), "Implicit conversion failed.");
    });

    UnitTest g_test_Equality(g_group_SchemeBoolTests, "Equality", [](){
        CUT_ASSERT.isTrue(SCHEME_TRUE == SCHEME_TRUE, "Scheme true must equal itself!");

        CUT_ASSERT.isTrue(SCHEME_FALSE == SCHEME_FALSE, "Scheme false must equal itself!");

        CUT_ASSERT.isFalse(SCHEME_TRUE == SCHEME_FALSE, "Scheme true cannot equal scheme false!");
        CUT_ASSERT.isFalse(SCHEME_FALSE == SCHEME_TRUE, "Scheme true cannot equal scheme false!");
    });

    UnitTest g_test_Conversion(g_group_SchemeBoolTests, "ConversionToBool", [](){
        bool result = SCHEME_TRUE;
        CUT_ASSERT.isTrue(SCHEME_TRUE, "Auto conversion of SCHEME_TRUE to 'true' is broken!");
        CUT_ASSERT.isFalse(SCHEME_FALSE, "Auto conversion of SCHEME_FALSE to 'false' is broken!");

        CUT_ASSERT.isTrue(SCHEME_TRUE == SCHEME_TRUE, "SCHEME_TRUE == SCHEME_TRUE does not evaluate to a boolean true!");
        CUT_ASSERT.isTrue(SCHEME_FALSE == SCHEME_FALSE, "SCHEME_FALSE == SCHEME_FALSE does not evaluate to a boolean true!");

        CUT_ASSERT.isFalse(SCHEME_TRUE != SCHEME_TRUE, "SCHEME_TRUE != SCHEME_TRUE does not evaluate to a boolean false!");
        CUT_ASSERT.isFalse(SCHEME_FALSE != SCHEME_FALSE, "SCHEME_FALSE != SCHEME_FALSE does not evaluate to a boolean alse!");
    });

    UnitTest g_test_ToString(g_group_SchemeBoolTests, "ToString", [](){
        CUT_ASSERT.isTrue(SCHEME_TRUE.toString().IsEqual("#t"), "SCHEME_TRUE has the wrong string representation!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.toString().IsEqual("#f"), "SCHEME_FALSE has the wrong string representation!");
    });

}
