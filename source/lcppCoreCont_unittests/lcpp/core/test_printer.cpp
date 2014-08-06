#include "stdafx.h"
#include "lcpp/core/printer.h"

#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/number.h"

LCPP_TestGroup(Printer);

LCPP_TestCase(Printer, Basics)
{
    auto pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    auto pContCall = cont::create(pContMain, &printer::print);
    cont::getStack(pContCall)->push(number::create(42));

    cont::trampoline(pContCall);

    auto pResult = cont::getStack(pContMain)->get(-1);

    CUT_ASSERT.isTrue(isVoid(pResult));
}
