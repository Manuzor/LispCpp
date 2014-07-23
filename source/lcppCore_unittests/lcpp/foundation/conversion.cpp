#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("Conversion");

    UnitTest g_test1(g_group, "ToString", []{
        {
            auto str = lcpp::toString(1);
            CUT_ASSERT.isTrue(str.IsEqual("1"));
        }
        {
            auto str = toString(1U);
            CUT_ASSERT.isTrue(str.IsEqual("1"));
        }
        {
            auto str = toString(1.0f);
            CUT_ASSERT.isTrue(str.IsEqual("1"));

            str = toString(1.1f);
            CUT_ASSERT.isTrue(str.IsEqual("1.1"));
        }
        {
            auto str = toString(1.0);
            CUT_ASSERT.isTrue(str.IsEqual("1"));

            str = toString(1.1);
            CUT_ASSERT.isTrue(str.IsEqual("1.1"));
        }
    });

    UnitTest g_test2(g_group, "To integer", []{
        {
            ezString str("42");
            ezInt64 integer;
            ezResult result = to(str, integer);
            CUT_ASSERT.isTrue(result.Succeeded(), "Conversion failed.");
            CUT_ASSERT.isTrue(integer == 42, "Conversion yields wrong result.");
        }
    });

    UnitTest g_test3(g_group, "To number (float)", []{
        {
            ezString str("3.1415");
            double value;
            ezResult result = to(str, value);
            CUT_ASSERT.isTrue(result.Succeeded(), "Conversion failed.");
            CUT_ASSERT.isTrue(value == 3.1415, "Conversion yields wrong result.");
        }
        {
            ezString str("3.000");
            double value;
            ezResult result = to(str, value);
            CUT_ASSERT.isTrue(result.Succeeded(), "Conversion failed.");
            CUT_ASSERT.isTrue(value == 3.000, "Conversion yields wrong result.");
        }
        {
            ezString str("1337.");
            double value;
            ezResult result = to(str, value);
            CUT_ASSERT.isTrue(result.Succeeded(), "Conversion failed.");
            CUT_ASSERT.isTrue(value == 1337, "Conversion yields wrong result.");
        }
    });
}
