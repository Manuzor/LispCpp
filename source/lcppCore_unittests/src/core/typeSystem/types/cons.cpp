#include "stdafx.h"
#include <functional>

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("SchemeConsTests");

    UnitTest g_test1(g_group, "Construction", [](){
        // Default construction
        {
            SchemeCons cons;
            CUT_ASSERT.isTrue(cons.car() == SCHEME_NIL, "Car is not nil!");
            CUT_ASSERT.isTrue(cons.cdr() == SCHEME_NIL, "Cdr is not nil!");
        }

        // Only car given
        {
            SchemeBool t(SCHEME_TRUE);
            SchemeCons cons(t);
            CUT_ASSERT.isTrue(cons.car() == SCHEME_TRUE, "Wrong object for car!");
            CUT_ASSERT.isTrue(cons.cdr() == SCHEME_NIL, "Cdr should be nil here!");
        }

        // Car and cdr explicitly given
        {
            SchemeBool t(SCHEME_TRUE);
            SchemeBool f(SCHEME_FALSE);
            SchemeCons cons(t, f);
            CUT_ASSERT.isTrue(cons.car() == SCHEME_TRUE, "Wrong car!");
            CUT_ASSERT.isTrue(cons.cdr() == SCHEME_FALSE, "Wrong cdr!");
        }
    });

    UnitTest g_test2(g_group, "CopyCtorAndCopyAssign", [](){
        {
            SchemeBool t(SCHEME_TRUE);

            SchemeCons first(t);
            SchemeCons second = first;
            CUT_ASSERT.isTrue(first.car() == SCHEME_TRUE);
            CUT_ASSERT.isTrue(second.car() == SCHEME_TRUE);
        }
        {
            SchemeInteger theInt(42);
            SchemeCons first(theInt);
            SchemeCons second = first;
            CUT_ASSERT.isTrue(first.car() == theInt);
            CUT_ASSERT.isTrue(second.car() == theInt, "Failed to construct 'second' by assigning it 'first'!");
            CUT_ASSERT.isTrue(&first.car() != &second.car(), "Cons is not supposed to make a flat copy!");
        }
    });

    UnitTest g_test3(g_group, "ToString", [](){
        static SchemeInteger one(1);
        static SchemeInteger two(2);
        static SchemeInteger three(3);

        struct TestWrapper
        {
            const char* expected;
            const SchemeCons cons;

            TestWrapper(const char* expected, const SchemeCons cons) :
                expected(expected),
                cons(cons)
            {
            }
        };

        ezDynamicArray<TestWrapper> tests;
        tests.Reserve(22);

        auto push = [&](const char* a, const SchemeCons& b)
        {
            tests.PushBack(TestWrapper(a, b));
        };

        push("(())", SchemeCons(SCHEME_NIL, SCHEME_NIL));
        push("(1)", SchemeCons(one, SCHEME_NIL));
        push("(() . 2)", SchemeCons(SCHEME_NIL, two));
        push("(1 . 2)", SchemeCons(one, two));

        push("(() ())", SchemeCons(SCHEME_NIL, SchemeCons(SCHEME_NIL, SCHEME_NIL)));
        push("(1 ())", SchemeCons(one, SchemeCons(SCHEME_NIL, SCHEME_NIL)));
        push("(1 2)", SchemeCons(one, SchemeCons(two, SCHEME_NIL)));
        push("(1 () . 3)", SchemeCons(one, SchemeCons(SCHEME_NIL, three)));
        push("(1 2 . 3)", SchemeCons(one, SchemeCons(two, three)));
        push("(() 2)", SchemeCons(SCHEME_NIL, SchemeCons(two, SCHEME_NIL)));
        push("(() () . 3)", SchemeCons(SCHEME_NIL, SchemeCons(SCHEME_NIL, three)));
        push("(() 2 . 3)", SchemeCons(SCHEME_NIL, SchemeCons(two, three)));
        push("(1 2 . 3)", SchemeCons(one, SchemeCons(two, three)));

        push("((()))", SchemeCons(SchemeCons(SCHEME_NIL, SCHEME_NIL), SCHEME_NIL));
        push("((()) . 1)", SchemeCons(SchemeCons(SCHEME_NIL, SCHEME_NIL), one));
        push("((2) . 1)", SchemeCons(SchemeCons(two, SCHEME_NIL), one));
        push("((() . 3) . 1)", SchemeCons(SchemeCons(SCHEME_NIL, three), one));
        push("((2 . 3) . 1)", SchemeCons(SchemeCons(two, three), one));
        push("((2))", SchemeCons(SchemeCons(two, SCHEME_NIL), SCHEME_NIL));
        push("((() . 3))", SchemeCons(SchemeCons(SCHEME_NIL, three), SCHEME_NIL));
        push("((2 . 3))", SchemeCons(SchemeCons(two, three), SCHEME_NIL));
        push("((2 . 3) . 1)", SchemeCons(SchemeCons(two, three), one));

        for(const auto& testWrapper : tests)
        {
            const char* expected = testWrapper.expected;
            const char* actual = testWrapper.cons.toString().GetData();
            CUT_ASSERT.isTrue(ezStringUtils::IsEqual(expected, actual));
        }
    });

    UnitTest g_test4(g_group, "ConstructDifferentTypes", [](){
        SchemeInteger number(42);
        SchemeBool t(SCHEME_TRUE);
        SchemeBool f(SCHEME_FALSE);
        SchemeNil nil(SCHEME_NIL);
        SchemeVoid v(SCHEME_VOID);

        {
            SchemeCons cons1(number);
            SchemeCons cons2(number, number);
        }
        {
            SchemeCons cons1(t);
            SchemeCons cons2(t, t);
        }
        {
            SchemeCons cons1(f);
            SchemeCons cons2(f, f);
        }
        {
            SchemeCons cons1(nil);
            SchemeCons cons2(nil, nil);
        }
        {
            SchemeCons cons1(v);
            SchemeCons cons2(v, v);
        }
    });
}
