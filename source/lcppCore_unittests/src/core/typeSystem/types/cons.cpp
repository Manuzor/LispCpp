#include "stdafx.h"
#include <functional>
#include "testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("SchemeConsTests");

    UnitTest g_test1(g_group, "Construction", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        // Default construction
        {
            auto cons = factory.createCons(SCHEME_NIL_PTR, SCHEME_NIL_PTR);
            CUT_ASSERT.isTrue(*cons->car() == SCHEME_NIL, "Car is not nil!");
            CUT_ASSERT.isTrue(*cons->cdr() == SCHEME_NIL, "Cdr is not nil!");
        }

        // Only car given
        {
            Ptr<SchemeBool> pT = &SCHEME_TRUE;
            auto cons = factory.createCons(pT, SCHEME_NIL_PTR);
            CUT_ASSERT.isTrue(*cons->car() == SCHEME_TRUE, "Wrong object for car!");
            CUT_ASSERT.isTrue(*cons->cdr() == SCHEME_NIL, "Cdr should be nil here!");
        }

        // Car and cdr explicitly given
        {
            auto t = &SCHEME_TRUE;
            auto f = &SCHEME_FALSE;
            auto cons = factory.createCons(t, f);
            CUT_ASSERT.isTrue(*cons->car() == SCHEME_TRUE, "Wrong car!");
            CUT_ASSERT.isTrue(*cons->cdr() == SCHEME_FALSE, "Wrong cdr!");
        }
    });

    UnitTest g_test2(g_group, "CopyCtorAndCopyAssign", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        {
            auto t = &SCHEME_TRUE;

            auto first = factory.createCons(t, SCHEME_NIL_PTR);
            SchemeCons second = *first;
            CUT_ASSERT.isTrue(*first->car() == SCHEME_TRUE);
            CUT_ASSERT.isTrue(*second.car() == SCHEME_TRUE);
        }
        {
            auto theInt = factory.createInteger(42);
            auto first = factory.createCons(theInt, SCHEME_NIL_PTR);
            SchemeCons second = *first;
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
            auto& test = factory.createCons(SCHEME_NIL_PTR, SCHEME_NIL_PTR);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(())"));
        }
        {
            auto& test = factory.createCons(one, SCHEME_NIL_PTR);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1)"));
        }
        {
            auto& test = factory.createCons(SCHEME_NIL_PTR, two);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() . 2)"));
        }
        {
            auto& test = factory.createCons(one, two);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 . 2)"));
        }
        {
            auto& inner = factory.createCons(SCHEME_NIL_PTR, SCHEME_NIL_PTR);
            auto& test = factory.createCons(SCHEME_NIL_PTR, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() ())"));
        }
        {
            auto& inner = factory.createCons(SCHEME_NIL_PTR, SCHEME_NIL_PTR);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 ())"));
        }
        {
            auto& inner = factory.createCons(two, SCHEME_NIL_PTR);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 2)"));
        }
        {
            auto& inner = factory.createCons(SCHEME_NIL_PTR, three);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 () . 3)"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 2 . 3)"));
        }
        {
            auto& inner = factory.createCons(two, SCHEME_NIL_PTR);
            auto& test = factory.createCons(SCHEME_NIL_PTR, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() 2)"));
        }
        {
            auto& inner = factory.createCons(SCHEME_NIL_PTR, three);
            auto& test = factory.createCons(SCHEME_NIL_PTR, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() () . 3)"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(SCHEME_NIL_PTR, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(() 2 . 3)"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(one, inner);
            CUT_ASSERT.isTrue(test->toString().IsEqual("(1 2 . 3)"));
        }
        {
            auto& inner = factory.createCons(SCHEME_NIL_PTR, SCHEME_NIL_PTR);
            auto& test = factory.createCons(inner, SCHEME_NIL_PTR);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((()))"));
        }
        {
            auto& inner = factory.createCons(SCHEME_NIL_PTR, SCHEME_NIL_PTR);
            auto& test = factory.createCons(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((()) . 1)"));
        }
        {
            auto& inner = factory.createCons(two, SCHEME_NIL_PTR);
            auto& test = factory.createCons(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2) . 1)"));
        }
        {
            auto& inner = factory.createCons(SCHEME_NIL_PTR, three);
            auto& test = factory.createCons(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((() . 3) . 1)"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(inner, one);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2 . 3) . 1)"));
        }
        {
            auto& inner = factory.createCons(two, SCHEME_NIL_PTR);
            auto& test = factory.createCons(inner, SCHEME_NIL_PTR);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2))"));
        }
        {
            auto& inner = factory.createCons(SCHEME_NIL_PTR, three);
            auto& test = factory.createCons(inner, SCHEME_NIL_PTR);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((() . 3))"));
        }
        {
            auto& inner = factory.createCons(two, three);
            auto& test = factory.createCons(inner, SCHEME_NIL_PTR);
            CUT_ASSERT.isTrue(test->toString().IsEqual("((2 . 3))"));
        }
    });

    UnitTest g_test4(g_group, "ConstructDifferentTypes", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();

        auto integer = factory.createInteger(42);
        auto t = &SCHEME_TRUE;
        auto f = &SCHEME_FALSE;
        auto nil = SCHEME_NIL_PTR;
        auto v = &SCHEME_VOID;

        {
            auto cons1 = factory.createCons(integer, SCHEME_NIL_PTR);
            auto cons2 = factory.createCons(integer, integer);
        }
        {
            auto cons1 = factory.createCons(t, SCHEME_NIL_PTR);
            auto cons2 = factory.createCons(t, t);
        }
        {
            auto cons1 = factory.createCons(f, SCHEME_NIL_PTR);
            auto cons2 = factory.createCons(f, f);
        }
        {
            auto cons1 = factory.createCons(nil, SCHEME_NIL_PTR);
            auto cons2 = factory.createCons(nil, nil);
        }
        {
            auto cons1 = factory.createCons(v, SCHEME_NIL_PTR);
            auto cons2 = factory.createCons(v, v);
        }
    });

    UnitTest g_test5(g_group, "Count", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        ezUInt32 numElements = 0;

        // Regular lists
        {
            auto cons = factory.createCons(SCHEME_NIL_PTR, SCHEME_NIL_PTR);
            CUT_ASSERT.isTrue(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 1, format("Expected count() to return 1, got %u!", numElements));
        }

        {
            auto cons = factory.createCons(SCHEME_NIL_PTR, factory.createCons(SCHEME_NIL_PTR, SCHEME_NIL_PTR));
            CUT_ASSERT.isTrue(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 2, format("Expected count() to return 2, got %u!", numElements));
        }

        {
            auto cons = factory.createCons(SCHEME_NIL_PTR, factory.createCons(SCHEME_NIL_PTR, factory.createCons(SCHEME_NIL_PTR, SCHEME_NIL_PTR)));
            CUT_ASSERT.isTrue(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 3, format("Expected count() to return 3, got %u!", numElements));
        }

        // Irregular lists
        {
            auto cons = factory.createCons(SCHEME_NIL_PTR, factory.createCons(SCHEME_NIL_PTR, SCHEME_VOID_PTR));
            CUT_ASSERT.isFalse(count(cons, numElements).IsSuccess(), "count() did not recognize (()) as regular list!");
            CUT_ASSERT.isTrue(numElements == 2, format("Expected count() to return 3, got %u!", numElements));
        }

    });
}
