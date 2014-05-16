#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("SchemeSymbolTests");

    UnitTest g_test1(g_group, "Type", [](){
        SchemeSymbol s1("a");
        SchemeSymbol s2("b");

        CUT_ASSERT.isTrue(s1.type() == s2.type(), "Different instances of scheme symbol do not have the same type!");
    });

    UnitTest g_test2(g_group, "Construction", [](){
        ezString str("Hello World");
        const char* cstr = "Goodbye Cruel World";

        SchemeSymbol s1(str);
        SchemeSymbol s2(cstr);

        CUT_ASSERT.isTrue(s1.value().IsEqual(str.GetData()), "Construction from another ezString failed.");
        CUT_ASSERT.isTrue(s2.value().IsEqual(cstr), "Construction from a c-string failed.");
    });

    UnitTest g_test3(g_group, "Equality", [](){
        // == with another symbol
        {
            SchemeSymbol s1("Hello World");
            SchemeSymbol s2("Hello World");

            CUT_ASSERT.isTrue(s1 == s2, "Equality operator for comparing two symbols is not working!");
            CUT_ASSERT.isTrue(s2 == s1, "Equality operator for comparing two symbols is not commutative!");
        }

        // == with a c-string
        {
            const char* cstr = "Hello World";
            SchemeSymbol s1("Hello World");

            CUT_ASSERT.isTrue(s1 == cstr, "Equality operator for comparing a scheme symbol with a c-string is not working!");
            CUT_ASSERT.isTrue(cstr == s1, "Equality operator for comparing a scheme symbol with a c-string is not commutative!");
        }

        // == with an ezString
        {
            ezString str = "Hello World";
            SchemeSymbol s1("Hello World");

            CUT_ASSERT.isTrue(s1 == str, "Equality operator for comparing a scheme symbol with an ezString is not working!");
            CUT_ASSERT.isTrue(str == s1, "Equality operator for comparing a scheme symbol with an ezString is not commutative!");
        }
    });

    UnitTest g_test4(g_group, "ConversionToString", [](){
        SchemeSymbol s1("Hello World");
        ezString str(s1);

        CUT_ASSERT.isTrue(s1.toString().IsEqual(str.GetData()), "s.toString should return the same thing as s.value()!");
        CUT_ASSERT.isTrue(str.IsEqual(s1.value().GetData()), "s.toString should return the same thing as s.value()!");
    });

    UnitTest g_test5(g_group, "ToString", [](){
        SchemeSymbol s1("Hello World");

        CUT_ASSERT.isTrue(s1.toString().IsEqual(s1.value().GetData()), "s.toString() should return the same thing as s.value()!");
    });
}
