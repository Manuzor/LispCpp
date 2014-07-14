#include "stdafx.h"
#include <functional>
#include "lcpp/testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("LispConsTests");

    UnitTest g_test1(g_group, "Construction", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        // Default construction
        {
            auto cons = LispCons::create(LCPP_NIL, LCPP_NIL);
            CUT_ASSERT.isTrue(cons->car() == LCPP_NIL, "Car is not nil!");
            CUT_ASSERT.isTrue(cons->cdr() == LCPP_NIL, "Cdr is not nil!");
        }

        // Only car given
        {
            auto pT = LCPP_TRUE;
            auto cons = LispCons::create(pT, LCPP_NIL);
            CUT_ASSERT.isTrue(cons->car() == pT, "Wrong object for car!");
            CUT_ASSERT.isTrue(cons->cdr() == LCPP_NIL, "Cdr should be nil here!");
        }

        // Car and cdr explicitly given
        {
            auto t = LCPP_TRUE;
            auto f = LCPP_FALSE;
            auto cons = LispCons::create(t, f);
            CUT_ASSERT.isTrue(cons->car() == t, "Wrong car!");
            CUT_ASSERT.isTrue(cons->cdr() == f, "Wrong cdr!");
        }
    });

    UnitTest g_test2(g_group, "CopyCtorAndCopyAssign", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        {
            auto t = LCPP_TRUE;

            auto first = LispCons::create(t, LCPP_NIL);
            LispCons second = *first;
            CUT_ASSERT.isTrue(first->car() == LCPP_TRUE);
            CUT_ASSERT.isTrue(second.car() == LCPP_TRUE);
        }
        {
            auto theInt = LispInteger::create(42);
            auto first = LispCons::create(theInt, LCPP_NIL);
            LispCons second = *first;
            CUT_ASSERT.isTrue(first->car() == theInt);
            CUT_ASSERT.isTrue(second.car() == theInt, "Failed to construct 'second' by assigning it 'first'!");
            CUT_ASSERT.isTrue(first->car() == second.car(), "Cons is not supposed to make a copy!");
        }
    });

    UnitTest g_test3(g_group, "ToString", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();

        auto& one = LispInteger::create(1);
        auto& two = LispInteger::create(2);
        auto& three = LispInteger::create(3);

        {
            auto& test = LispCons::create(LCPP_NIL, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(())"));
        }
        {
            auto& test = LispCons::create(one, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1)"));
        }
        {
            auto& test = LispCons::create(LCPP_NIL, two);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() . 2)"));
        }
        {
            auto& test = LispCons::create(one, two);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 . 2)"));
        }
        {
            auto& inner = LispCons::create(LCPP_NIL, LCPP_NIL);
            auto& test = LispCons::create(LCPP_NIL, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() ())"));
        }
        {
            auto& inner = LispCons::create(LCPP_NIL, LCPP_NIL);
            auto& test = LispCons::create(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 ())"));
        }
        {
            auto& inner = LispCons::create(two, LCPP_NIL);
            auto& test = LispCons::create(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 2)"));
        }
        {
            auto& inner = LispCons::create(LCPP_NIL, three);
            auto& test = LispCons::create(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 () . 3)"));
        }
        {
            auto& inner = LispCons::create(two, three);
            auto& test = LispCons::create(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 2 . 3)"));
        }
        {
            auto& inner = LispCons::create(two, LCPP_NIL);
            auto& test = LispCons::create(LCPP_NIL, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() 2)"));
        }
        {
            auto& inner = LispCons::create(LCPP_NIL, three);
            auto& test = LispCons::create(LCPP_NIL, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() () . 3)"));
        }
        {
            auto& inner = LispCons::create(two, three);
            auto& test = LispCons::create(LCPP_NIL, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() 2 . 3)"));
        }
        {
            auto& inner = LispCons::create(two, three);
            auto& test = LispCons::create(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 2 . 3)"));
        }
        {
            auto& inner = LispCons::create(LCPP_NIL, LCPP_NIL);
            auto& test = LispCons::create(inner, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((()))"));
        }
        {
            auto& inner = LispCons::create(LCPP_NIL, LCPP_NIL);
            auto& test = LispCons::create(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((()) . 1)"));
        }
        {
            auto& inner = LispCons::create(two, LCPP_NIL);
            auto& test = LispCons::create(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2) . 1)"));
        }
        {
            auto& inner = LispCons::create(LCPP_NIL, three);
            auto& test = LispCons::create(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((() . 3) . 1)"));
        }
        {
            auto& inner = LispCons::create(two, three);
            auto& test = LispCons::create(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2 . 3) . 1)"));
        }
        {
            auto& inner = LispCons::create(two, LCPP_NIL);
            auto& test = LispCons::create(inner, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2))"));
        }
        {
            auto& inner = LispCons::create(LCPP_NIL, three);
            auto& test = LispCons::create(inner, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((() . 3))"));
        }
        {
            auto& inner = LispCons::create(two, three);
            auto& test = LispCons::create(inner, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2 . 3))"));
        }
    });

    UnitTest g_test4(g_group, "ConstructDifferentTypes", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();

        auto pInteger = LispInteger::create(42);

        {
            auto cons1 = LispCons::create(pInteger, LCPP_NIL);
            auto cons2 = LispCons::create(pInteger, pInteger);
        }
        {
            auto cons1 = LispCons::create(LCPP_TRUE, LCPP_NIL);
            auto cons2 = LispCons::create(LCPP_TRUE, LCPP_TRUE);
        }
        {
            auto cons1 = LispCons::create(LCPP_FALSE, LCPP_NIL);
            auto cons2 = LispCons::create(LCPP_FALSE, LCPP_FALSE);
        }
        {
            auto cons1 = LispCons::create(LCPP_NIL, LCPP_NIL);
            auto cons2 = LispCons::create(LCPP_NIL, LCPP_NIL);
        }
        {
            auto cons1 = LispCons::create(LCPP_VOID, LCPP_NIL);
            auto cons2 = LispCons::create(LCPP_VOID, LCPP_VOID);
        }
    });

    UnitTest g_test5(g_group, "Count", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        ezUInt32 numElements = 0;

        // Regular lists
        {
            auto cons = LispCons::create(LCPP_NIL, LCPP_NIL);
            CUT_ASSERT.isTrue(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 1, format("Expected count() to return 1, got %u!", numElements));
        }

        {
            auto cons = LispCons::create(LCPP_NIL, LispCons::create(LCPP_NIL, LCPP_NIL));
            CUT_ASSERT.isTrue(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 2, format("Expected count() to return 2, got %u!", numElements));
        }

        {
            auto cons = LispCons::create(LCPP_NIL, LispCons::create(LCPP_NIL, LispCons::create(LCPP_NIL, LCPP_NIL)));
            CUT_ASSERT.isTrue(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 3, format("Expected count() to return 3, got %u!", numElements));
        }

        // Irregular lists
        {
            auto cons = LispCons::create(LCPP_NIL, LispCons::create(LCPP_NIL, LCPP_VOID));
            CUT_ASSERT.isFalse(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 2, format("Expected count() to return 2, got %u!", numElements));
        }

    });
}
