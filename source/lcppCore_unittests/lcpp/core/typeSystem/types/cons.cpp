﻿#include "stdafx.h"
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
            auto cons = factory.createCons(LCPP_NIL, LCPP_NIL);
            CUT_ASSERT.isTrue(cons->car() == LCPP_NIL, "Car is not nil!");
            CUT_ASSERT.isTrue(cons->cdr() == LCPP_NIL, "Cdr is not nil!");
        }

        // Only car given
        {
            auto pT = LCPP_TRUE;
            auto cons = factory.createCons(pT, LCPP_NIL);
            CUT_ASSERT.isTrue(cons->car() == pT, "Wrong object for car!");
            CUT_ASSERT.isTrue(cons->cdr() == LCPP_NIL, "Cdr should be nil here!");
        }

        // Car and cdr explicitly given
        {
            auto t = LCPP_TRUE;
            auto f = LCPP_FALSE;
            auto cons = factory.createCons(t, f);
            CUT_ASSERT.isTrue(cons->car() == t, "Wrong car!");
            CUT_ASSERT.isTrue(cons->cdr() == f, "Wrong cdr!");
        }
    });

    UnitTest g_test2(g_group, "CopyCtorAndCopyAssign", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        {
            auto t = LCPP_TRUE;

            auto first = factory.createCons(t, LCPP_NIL);
            LispCons second = *first;
            CUT_ASSERT.isTrue(first->car() == LCPP_TRUE);
            CUT_ASSERT.isTrue(second.car() == LCPP_TRUE);
        }
        {
            auto theInt = factory.createInteger(42);
            auto first = factory.createCons(theInt, LCPP_NIL);
            LispCons second = *first;
            CUT_ASSERT.isTrue(first->car() == theInt);
            CUT_ASSERT.isTrue(second.car() == theInt, "Failed to construct 'second' by assigning it 'first'!");
            CUT_ASSERT.isTrue(first->car() == second.car(), "Cons is not supposed to make a copy!");
        }
    });

    UnitTest g_test3(g_group, "ToString", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();

        auto& one = factory.createInteger(1);
        auto& two = factory.createInteger(2);
        auto& three = factory.createInteger(3);

        {
            auto& test = factory.createCons(LCPP_NIL, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(())"));
        }
        {
            auto& test = factory.createCons(one, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1)"));
        }
        {
            auto& test = factory.createCons(LCPP_NIL, two);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() . 2)"));
        }
        {
            auto& test = factory.createCons(one, two);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 . 2)"));
        }
        {
            auto& inner = factory.createCons(LCPP_NIL, LCPP_NIL);
            auto& test = factory.createCons(LCPP_NIL, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() ())"));
        }
        {
            auto& inner = factory.createCons(LCPP_NIL, LCPP_NIL);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 ())"));
        }
        {
            auto& inner = factory.createCons(two, LCPP_NIL);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 2)"));
        }
        {
            auto& inner = factory.createCons(LCPP_NIL, three);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 () . 3)"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 2 . 3)"));
        }
        {
            auto& inner = factory.createCons(two, LCPP_NIL);
            auto& test = factory.createCons(LCPP_NIL, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() 2)"));
        }
        {
            auto& inner = factory.createCons(LCPP_NIL, three);
            auto& test = factory.createCons(LCPP_NIL, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() () . 3)"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(LCPP_NIL, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() 2 . 3)"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 2 . 3)"));
        }
        {
            auto& inner = factory.createCons(LCPP_NIL, LCPP_NIL);
            auto& test = factory.createCons(inner, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((()))"));
        }
        {
            auto& inner = factory.createCons(LCPP_NIL, LCPP_NIL);
            auto& test = factory.createCons(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((()) . 1)"));
        }
        {
            auto& inner = factory.createCons(two, LCPP_NIL);
            auto& test = factory.createCons(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2) . 1)"));
        }
        {
            auto& inner = factory.createCons(LCPP_NIL, three);
            auto& test = factory.createCons(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((() . 3) . 1)"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2 . 3) . 1)"));
        }
        {
            auto& inner = factory.createCons(two, LCPP_NIL);
            auto& test = factory.createCons(inner, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2))"));
        }
        {
            auto& inner = factory.createCons(LCPP_NIL, three);
            auto& test = factory.createCons(inner, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((() . 3))"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(inner, LCPP_NIL);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2 . 3))"));
        }
    });

    UnitTest g_test4(g_group, "ConstructDifferentTypes", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();

        auto pInteger = factory.createInteger(42);

        {
            auto cons1 = factory.createCons(pInteger, LCPP_NIL);
            auto cons2 = factory.createCons(pInteger, pInteger);
        }
        {
            auto cons1 = factory.createCons(LCPP_TRUE, LCPP_NIL);
            auto cons2 = factory.createCons(LCPP_TRUE, LCPP_TRUE);
        }
        {
            auto cons1 = factory.createCons(LCPP_FALSE, LCPP_NIL);
            auto cons2 = factory.createCons(LCPP_FALSE, LCPP_FALSE);
        }
        {
            auto cons1 = factory.createCons(LCPP_NIL, LCPP_NIL);
            auto cons2 = factory.createCons(LCPP_NIL, LCPP_NIL);
        }
        {
            auto cons1 = factory.createCons(LCPP_VOID, LCPP_NIL);
            auto cons2 = factory.createCons(LCPP_VOID, LCPP_VOID);
        }
    });

    UnitTest g_test5(g_group, "Count", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        ezUInt32 numElements = 0;

        // Regular lists
        {
            auto cons = factory.createCons(LCPP_NIL, LCPP_NIL);
            CUT_ASSERT.isTrue(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 1, format("Expected count() to return 1, got %u!", numElements));
        }

        {
            auto cons = factory.createCons(LCPP_NIL, factory.createCons(LCPP_NIL, LCPP_NIL));
            CUT_ASSERT.isTrue(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 2, format("Expected count() to return 2, got %u!", numElements));
        }

        {
            auto cons = factory.createCons(LCPP_NIL, factory.createCons(LCPP_NIL, factory.createCons(LCPP_NIL, LCPP_NIL)));
            CUT_ASSERT.isTrue(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 3, format("Expected count() to return 3, got %u!", numElements));
        }

        // Irregular lists
        {
            auto cons = factory.createCons(LCPP_NIL, factory.createCons(LCPP_NIL, LCPP_VOID));
            CUT_ASSERT.isFalse(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 2, format("Expected count() to return 3, got %u!", numElements));
        }

    });
}
