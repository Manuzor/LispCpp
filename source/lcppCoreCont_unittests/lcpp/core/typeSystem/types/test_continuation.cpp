#include "stdafx.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"

namespace lcpp
{
    static auto g_sum = number::Integer_t(0);

    static Ptr<LispObject> accumulator(Ptr<LispObject> pCont)
    {
        auto pStack = cont::getStack(pCont);

        auto pToGo = pStack->get(-1);
        auto value = number::getInteger(pToGo);

        if(value <= 0)
        {
            return cont::getParent(pCont);
        }

        ++g_sum;

        pStack->pop();
        pStack->push(number::create(value - 1));

        return pCont;
    }

    static Ptr<LispObject> entry(Ptr<LispObject> pCont)
    {
        CUT_ASSERT.isTrue(g_sum == 3);

        return cont::getParent(pCont);
    }
}

LCPP_TestGroup(Continuation);

LCPP_TestCase(Continuation, Basics)
{
    auto pInteger_1 = number::create(1);
    auto pInteger_2 = number::create(2);
    auto pInteger_3 = number::create(3);

    auto pContRoot = cont::createTopLevel(&entry);
    auto pContChild = cont::create(pContRoot, &accumulator);

    cont::getStack(pContChild)->push(number::create(3)); // Do 3 'iterations'

    g_sum = 0;

    cont::trampoline(pContChild);
}
