#include "stdafx.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"

namespace lcpp
{
    static Ptr<LispObject> sorter(Ptr<LispObject> pCont)
    {
        return cont::getParent(pCont);
    }

    static Ptr<LispObject> entry(Ptr<LispObject> pCont)
    {
        return cont::getParent(pCont);
    }
}

LCPP_TestGroup(Continuation);

LCPP_TestCase(Continuation, Basics)
{
    auto pInteger_1 = number::create(1);
    auto pInteger_2 = number::create(2);
    auto pInteger_3 = number::create(3);

    auto pRootCont = cont::createTopLevel(&entry);
    auto pSorterCont = cont::create(pRootCont, &entry);

    cont::trampoline(pSorterCont);
}
