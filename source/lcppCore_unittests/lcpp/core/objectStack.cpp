#include "stdafx.h"
#include "lcpp/testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("LispObjectStackTests");

    UnitTest g_test1(g_group, "EmptyStack", []()
    {
        auto pStack = LispObjectStack::create();

        CUT_ASSERT.isTrue(pStack->isEmpty(), "A new stack instance is supposed to be empty.");
        CUT_ASSERT.isTrue(pStack->size() == 0, "The size of a new stack instance is supposed to be 0.");
        CUT_ASSERT.isTrue(pStack->top() == 0, "The top of a new stack instance is supposed to be 0.");
        CUT_ASSERT.throwsNothing([&]{ pStack->pop(); }, "Pop on an empty stack should not throw an exception.");
        CUT_ASSERT.isFalse(pStack->pop().Succeeded(), "Pop on an empty stack should yield EZ_FAILURE.");
        CUT_ASSERT.throwsNothing([&]{ pStack->get(-1); }, "Get on an empty stack should not throw an exception.");
        CUT_ASSERT.isTrue(isNil(pStack->get(-1)), "Get on an empty stack should always yield LCPP_NIL, regardless of the index argument.");
        CUT_ASSERT.isTrue(isNil(pStack->get(0)), "Get on an empty stack should always yield LCPP_NIL, regardless of the index argument.");
        CUT_ASSERT.isTrue(isNil(pStack->get(std::numeric_limits<ezInt32>::max())), "Get on an empty stack should always yield LCPP_NIL, regardless of the index argument.");
    });

    UnitTest g_test2(g_group, "Manipulation", []()
    {
        auto pStack = LispObjectStack::create();
        auto one   = LispInteger::create(1);
        auto two   = LispInteger::create(2);
        auto three = LispInteger::create(3);
        auto four  = LispInteger::create(4);

        pStack->push(one);

        CUT_ASSERT.isFalse(pStack->isEmpty());
        CUT_ASSERT.isTrue(pStack->size() == 1);
        CUT_ASSERT.isTrue(pStack->top() == 0);
        CUT_ASSERT.isTrue(pStack->get(0) == one);
        CUT_ASSERT.isTrue(pStack->get(-1) == one, "Access with relative index -1 failed.");
        CUT_ASSERT.isFalse(pStack->get(-1) == two);

        pStack->push(two);
        pStack->push(three);
        pStack->push(four);

        CUT_ASSERT.isTrue(pStack->get(-1) == four);
        CUT_ASSERT.isTrue(pStack->get( 3) == four);
        CUT_ASSERT.isTrue(pStack->get(-2) == three);
        CUT_ASSERT.isTrue(pStack->get( 2) == three);
        CUT_ASSERT.isTrue(pStack->get(-3) == two);
        CUT_ASSERT.isTrue(pStack->get( 1) == two);
        CUT_ASSERT.isTrue(pStack->get(-4) == one);
        CUT_ASSERT.isTrue(pStack->get(-5) == LCPP_NIL);

        // Clear the stack

        // pop 'four'
        CUT_ASSERT.isTrue(pStack->pop().Succeeded());
        CUT_ASSERT.isTrue(pStack->get(-1) == three);

        // pop 'three'
        CUT_ASSERT.isTrue(pStack->pop().Succeeded());
        CUT_ASSERT.isTrue(pStack->get(-1) == two);

        // pop 'two'
        CUT_ASSERT.isTrue(pStack->pop().Succeeded());
        CUT_ASSERT.isTrue(pStack->get(-1) == one);
        CUT_ASSERT.isFalse(pStack->isEmpty());

        // pop 'one'
        CUT_ASSERT.isTrue(pStack->pop().Succeeded());
        CUT_ASSERT.isTrue(pStack->get(-1) == LCPP_NIL);
        CUT_ASSERT.isTrue(pStack->isEmpty());
    });
}
