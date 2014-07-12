#include "stdafx.h"
#include "lcpp/testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("LispSymbolTests");

    UnitTest g_test1(g_group, "Type", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        auto s1 = factory.createSymbol("a");
        auto s2 = factory.createSymbol("b");

        CUT_ASSERT.isTrue(s1->type() == s2->type(), "Different instances of scheme symbol do not have the same type!");
    });

    UnitTest g_test2(g_group, "Construction", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        ezString str("Hello World");
        const char* cstr = "Goodbye Cruel World";

        auto s1 = factory.createSymbol(str);
        auto s2 = factory.createSymbol(cstr);

        CUT_ASSERT.isTrue(s1->value().IsEqual(str.GetData()), "Construction from another ezString failed.");
        CUT_ASSERT.isTrue(s2->value().IsEqual(cstr), "Construction from a c-string failed.");
    });

    UnitTest g_test3(g_group, "Equality", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        // == with another symbol
        {
            auto s1 = factory.createSymbol("Hello World");
            auto s2 = factory.createSymbol("Hello World");

            CUT_ASSERT.isTrue(s1 == s2, "Equality operator for comparing two symbols is not working!");
            CUT_ASSERT.isTrue(s2 == s1, "Equality operator for comparing two symbols is not commutative!");
        }

        // with a c-string
        {
            const char* cstr = "Hello World";
            auto s1 = factory.createSymbol("Hello World");

            CUT_ASSERT.isTrue(s1->value().IsEqual(cstr), "Equality operator for comparing a scheme symbol with a c-string is not working!");
        }
    });

    UnitTest g_test4(g_group, "ToString", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        auto s1 = factory.createSymbol("Hello World");

        CUT_ASSERT.isTrue(s1->toString().IsEqual(s1->value().GetData()), "s.toString() should return the same thing as s->value()!");
    });

    UnitTest g_test6(g_group, "Instancing", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        auto s1 = factory.createSymbol("hello");
        auto s2 = factory.createSymbol("hello");

        CUT_ASSERT.isTrue(s1 == s2, "Symbols with the same value should have the same identity!");
    });
}
