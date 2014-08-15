#include "stdafx.h"
#include "lcpp/foundation/containers/stackBase.h"

using namespace cut;
using namespace lcpp;

namespace
{
    class TestStack : public StackBase<ezInt32, 16>
    {
    public:

        TestStack() : StackBase(defaultAllocator()) {}
    };

    UnitTestGroup g_group("StackBase");

    UnitTest g_test1(g_group, "Basics", []{
        auto stack = TestStack();

        stack.push(0);
        stack.push(1);
        stack.push(2);
        stack.push(3);
        stack.push(4);

        CUT_ASSERT.isTrue(stack.size() == 5);

        CUT_ASSERT.isTrue(stack.get(0) == 0);
        CUT_ASSERT.isTrue(stack.get(-5) == 0);

        CUT_ASSERT.isTrue(stack.get(1) == 1);
        CUT_ASSERT.isTrue(stack.get(-4) == 1);

        CUT_ASSERT.isTrue(stack.get(2) == 2);
        CUT_ASSERT.isTrue(stack.get(-3) == 2);

        CUT_ASSERT.isTrue(stack.get(3) == 3);
        CUT_ASSERT.isTrue(stack.get(-2) == 3);

        CUT_ASSERT.isTrue(stack.get(4) == 4);
        CUT_ASSERT.isTrue(stack.get(-1) == 4);

        //////////////////////////////////////////////////////////////////////////
        
        stack.pop();

        CUT_ASSERT.isTrue(stack.size() == 4);

        CUT_ASSERT.isTrue(stack.get(0) == 0);
        CUT_ASSERT.isTrue(stack.get(-4) == 0);

        CUT_ASSERT.isTrue(stack.get(1) == 1);
        CUT_ASSERT.isTrue(stack.get(-3) == 1);

        CUT_ASSERT.isTrue(stack.get(2) == 2);
        CUT_ASSERT.isTrue(stack.get(-2) == 2);

        CUT_ASSERT.isTrue(stack.get(3) == 3);
        CUT_ASSERT.isTrue(stack.get(-1) == 3);

        //////////////////////////////////////////////////////////////////////////
        
        stack.pop(0);

        CUT_ASSERT.isTrue(stack.size() == 3);

        CUT_ASSERT.isTrue(stack.get(0) == 1);
        CUT_ASSERT.isTrue(stack.get(-3) == 1);

        CUT_ASSERT.isTrue(stack.get(1) == 2);
        CUT_ASSERT.isTrue(stack.get(-2) == 2);

        CUT_ASSERT.isTrue(stack.get(2) == 3);
        CUT_ASSERT.isTrue(stack.get(-1) == 3);
    });
}
