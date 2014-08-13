#include "stdafx.h"
#include "lcpp/core/printer.h"

#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/runtime.h"

LCPP_TestGroup(Printer);

LCPP_TestCase(Printer, StandardOutput)
{
    auto pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    auto pContCall = cont::create(pContMain, &printer::print);
    cont::getStack(pContCall)->push(number::create(42));

    cont::trampoline(pContCall);

    auto pResult = cont::getStack(pContMain)->get(-1);

    CUT_ASSERT.isTrue(isVoid(pResult));
}

LCPP_TestCase(Printer, CustomStream)
{
    auto stream = TestStringStream();
    auto pState = LCPP_test_pRuntimeState;
    pState->getPrinterState()->m_pOutStream = &stream;

    auto pContMain = cont::createTopLevel(pState);
    auto pContCall = cont::create(pContMain, &printer::print);
    cont::getStack(pContCall)->push(number::create(42));

    CUT_ASSERT.isTrue(stream.m_content.IsEmpty());

    cont::trampoline(pContCall);

    auto pResult = cont::getStack(pContMain)->get(-1);

    CUT_ASSERT.isTrue(isVoid(pResult));

    CUT_ASSERT.isTrue(stream.m_content.IsEqual("42"));
}
