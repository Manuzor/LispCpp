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
            SchemeCons cons(SCHEME_NIL, SCHEME_NIL);
            CUT_ASSERT.isTrue(cons.car() == SCHEME_NIL, "Car is not nil!");
            CUT_ASSERT.isTrue(cons.cdr() == SCHEME_NIL, "Cdr is not nil!");
        }

        // Only car given
        {
            SchemeBool t(SCHEME_TRUE);
            SchemeCons cons(t, SCHEME_NIL);
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

            SchemeCons first(t, SCHEME_NIL);
            SchemeCons second = first;
            CUT_ASSERT.isTrue(first.car() == SCHEME_TRUE);
            CUT_ASSERT.isTrue(second.car() == SCHEME_TRUE);
        }
        {
            SchemeInteger theInt(42);
            SchemeCons first(theInt, SCHEME_NIL);
            SchemeCons second = first;
            CUT_ASSERT.isTrue(first.car() == theInt);
            CUT_ASSERT.isTrue(second.car() == theInt, "Failed to construct 'second' by assigning it 'first'!");
            CUT_ASSERT.isTrue(&first.car() == &second.car(), "Cons is not supposed to make a copy!");
        }
    });

    UnitTest g_test3(g_group, "ToString", [](){

        // More harmless tests first...
        {
            SchemeInteger one(1);
            SchemeInteger two(2);

            SchemeCons cons(SCHEME_NIL, SCHEME_NIL);
            ezString result;

            result = cons.toString();
            CUT_ASSERT.isTrue(result.IsEqual("(())"));
            cons.car(one);
            result = cons.toString();
            CUT_ASSERT.isTrue(result.IsEqual("(1)"));
            cons.cdr(two);
            result = cons.toString();
            CUT_ASSERT.isTrue(result.IsEqual("(1 . 2)"));
            cons.car(SCHEME_NIL);
            result = cons.toString();
            CUT_ASSERT.isTrue(result.IsEqual("(() . 2)"));
        }

        SchemeInteger one(1);
        SchemeInteger two(2);
        SchemeInteger three(3);

        {
            SchemeCons test(SCHEME_NIL, SCHEME_NIL);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(())"));
        }
        {
            SchemeCons test(one, SCHEME_NIL);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(1)"));
        }
        {
            SchemeCons test(SCHEME_NIL, two);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(() . 2)"));
        }
        {
            SchemeCons test(one, two);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(1 . 2)"));
        }
        {
            SchemeCons inner(SCHEME_NIL, SCHEME_NIL);
            SchemeCons test(SCHEME_NIL, inner);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(() ())"));
        }
        {
            SchemeCons inner(SCHEME_NIL, SCHEME_NIL);
            SchemeCons test(one, inner);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(1 ())"));
        }
        {
            SchemeCons inner(two, SCHEME_NIL);
            SchemeCons test(one, inner);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(1 2)"));
        }
        {
            SchemeCons inner(SCHEME_NIL, three);
            SchemeCons test(one, inner);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(1 () . 3)"));
        }
        {
            SchemeCons inner(two, three);
            SchemeCons test(one, inner);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(1 2 . 3)"));
        }
        {
            SchemeCons inner(two, SCHEME_NIL);
            SchemeCons test(SCHEME_NIL, inner);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(() 2)"));
        }
        {
            SchemeCons inner(SCHEME_NIL, three);
            SchemeCons test(SCHEME_NIL, inner);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(() () . 3)"));
        }
        {
            SchemeCons inner(two, three);
            SchemeCons test(SCHEME_NIL, inner);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(() 2 . 3)"));
        }
        {
            SchemeCons inner(two, three);
            SchemeCons test(one, inner);
            CUT_ASSERT.isTrue(test.toString().IsEqual("(1 2 . 3)"));
        }
        {
            SchemeCons inner(SCHEME_NIL, SCHEME_NIL);
            SchemeCons test(inner, SCHEME_NIL);
            CUT_ASSERT.isTrue(test.toString().IsEqual("((()))"));
        }
        {
            SchemeCons inner(SCHEME_NIL, SCHEME_NIL);
            SchemeCons test(inner, one);
            CUT_ASSERT.isTrue(test.toString().IsEqual("((()) . 1)"));
        }
        {
            SchemeCons inner(two, SCHEME_NIL);
            SchemeCons test(inner, one);
            CUT_ASSERT.isTrue(test.toString().IsEqual("((2) . 1)"));
        }
        {
            SchemeCons inner(SCHEME_NIL, three);
            SchemeCons test(inner, one);
            CUT_ASSERT.isTrue(test.toString().IsEqual("((() . 3) . 1)"));
        }
        {
            SchemeCons inner(two, three);
            SchemeCons test(inner, one);
            CUT_ASSERT.isTrue(test.toString().IsEqual("((2 . 3) . 1)"));
        }
        {
            SchemeCons inner(two, SCHEME_NIL);
            SchemeCons test(inner, SCHEME_NIL);
            CUT_ASSERT.isTrue(test.toString().IsEqual("((2))"));
        }
        {
            SchemeCons inner(SCHEME_NIL, three);
            SchemeCons test(inner, SCHEME_NIL);
            CUT_ASSERT.isTrue(test.toString().IsEqual("((() . 3))"));
        }
        {
            SchemeCons inner(two, three);
            SchemeCons test(inner, SCHEME_NIL);
            CUT_ASSERT.isTrue(test.toString().IsEqual("((2 . 3))"));
        }
    });

    UnitTest g_test4(g_group, "ConstructDifferentTypes", [](){
        SchemeInteger number(42);
        SchemeBool t(SCHEME_TRUE);
        SchemeBool f(SCHEME_FALSE);
        SchemeNil nil(SCHEME_NIL);
        SchemeVoid v(SCHEME_VOID);

        {
            SchemeCons cons1(number, SCHEME_NIL);
            SchemeCons cons2(number, number);
        }
        {
            SchemeCons cons1(t, SCHEME_NIL);
            SchemeCons cons2(t, t);
        }
        {
            SchemeCons cons1(f, SCHEME_NIL);
            SchemeCons cons2(f, f);
        }
        {
            SchemeCons cons1(nil, SCHEME_NIL);
            SchemeCons cons2(nil, nil);
        }
        {
            SchemeCons cons1(v, SCHEME_NIL);
            SchemeCons cons2(v, v);
        }
    });
}
