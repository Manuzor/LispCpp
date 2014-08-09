#include "stdafx.h"
#include "lcpp/core/printer.h"

#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    class MyStringStream :
        public ezStreamWriterBase
    {
    public:

        ezStringBuilder m_content;

        virtual ezResult WriteBytes(const void* pWriteBuffer, ezUInt64 uiBytesToWrite) override
        {
            auto pBuffer = reinterpret_cast<const ezUInt8*>(pWriteBuffer);

            for(auto i = ezUInt64(0); i < uiBytesToWrite; ++i)
            {
                m_content.Append(pBuffer[i]);
            }

            return EZ_SUCCESS;
        }

    };
}

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
    auto stream = MyStringStream();
    auto pState = LCPP_test_pRuntimeState;
    pState->getPrinterState()->m_pOutStream = &stream;

    auto pContMain = cont::createTopLevel(pState);
    auto pContCall = cont::create(pContMain, &printer::print);
    cont::getStack(pContCall)->push(number::create(42));

    CUT_ASSERT.isTrue(stream.m_content.IsEmpty());

    cont::trampoline(pContCall);

    auto pResult = cont::getStack(pContMain)->get(-1);

    CUT_ASSERT.isTrue(isVoid(pResult));

    CUT_ASSERT.isTrue(stream.m_content.IsEqual("42\n"));
}
