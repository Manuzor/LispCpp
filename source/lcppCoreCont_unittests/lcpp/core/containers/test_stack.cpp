#include "stdafx.h"
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/void.h"

LCPP_TEST_GROUP(Stack);

LCPP_TEST(Stack, EmptyStack)
{
    auto stack = Stack();
    auto pStack = &stack;

    CUT_ASSERT.isTrue(pStack->isEmpty(), "A new stack instance is supposed to be empty.");
    CUT_ASSERT.isTrue(pStack->size() == 0, "The size of a new stack instance is supposed to be 0.");
    CUT_ASSERT.isTrue(pStack->top() == 0, "The top of a new stack instance is supposed to be 0.");
    CUT_ASSERT.throwsNothing([&]{ pStack->pop(); }, "Pop on an empty stack should not throw an exception.");
    CUT_ASSERT.isFalse(pStack->pop().Succeeded(), "Pop on an empty stack should yield EZ_FAILURE.");
    CUT_ASSERT.throwsNothing([&]{ pStack->get(-1); }, "Get on an empty stack should not throw an exception.");
    CUT_ASSERT.isTrue(isNil(pStack->get(-1)), "Get on an empty stack should always yield LCPP_NIL, regardless of the index argument.");
    CUT_ASSERT.isTrue(isNil(pStack->get(0)), "Get on an empty stack should always yield LCPP_NIL, regardless of the index argument.");
    CUT_ASSERT.isTrue(isNil(pStack->get(std::numeric_limits<ezInt32>::max())), "Get on an empty stack should always yield LCPP_NIL, regardless of the index argument.");

}

LCPP_TEST(Stack, Manipulation)
{
    auto stack = Stack();
    auto pStack = &stack;
    auto one = LCPP_TRUE;
    auto two = LCPP_VOID;
    auto three = LCPP_FALSE;

    pStack->push(one);

    CUT_ASSERT.isFalse(pStack->isEmpty());
    CUT_ASSERT.isTrue(pStack->size() == 1);
    CUT_ASSERT.isTrue(pStack->top() == 0);
    CUT_ASSERT.isTrue(pStack->get(0) == one);
    CUT_ASSERT.isTrue(pStack->get(-1) == one, "Access with relative index -1 failed.");
    CUT_ASSERT.isFalse(pStack->get(-1) == two);

    pStack->push(two);
    pStack->push(three);

    CUT_ASSERT.isTrue(pStack->get(-1) == three);
    CUT_ASSERT.isTrue(pStack->get(2) == three);
    CUT_ASSERT.isTrue(pStack->get(-2) == two);
    CUT_ASSERT.isTrue(pStack->get(1) == two);
    CUT_ASSERT.isTrue(pStack->get(-3) == one);
    CUT_ASSERT.isTrue(pStack->get(-4) == LCPP_NIL);
    CUT_ASSERT.isTrue(pStack->get(3) == LCPP_NIL);

    // Clear the stack one by one

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
}

