#include "stdafx.h"
#include "lcpp/test/common.h"
#include "lcpp/core/runtime.h"

#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"

using namespace lcpp;
using namespace lcpp::test;

lcpp::test::UnitTest::UnitTest(cut::UnitTestGroup& group)
{
    group.registerUnitTest(this);
}

void lcpp::test::UnitTest::inititialize()
{
    static ElectricFenceAllocator allocator(defaultAllocator());
    LCPP_test_pRuntimeState->initialize(&allocator);
}

void lcpp::test::UnitTest::shutdown()
{
    // TODO shutdown stuff here on a per-test basis
    LCPP_test_pRuntimeState->shutdown();
}

lcpp::test::UnitTestNoInit::UnitTestNoInit(cut::UnitTestGroup& group)
{
    group.registerUnitTest(this);
}

ezUInt32 lcpp::test::detail::g_uiDebugBreakOnExceptions = 0;
ezUInt32 lcpp::test::detail::g_uiLogExceptions = 0;

EZ_ON_GLOBAL_EVENT(ThrowException)
{
    if (lcpp::test::detail::g_uiDebugBreakOnExceptions > 0)
    {
        auto pException = (lcpp::exceptions::ExceptionBase*)param0.Get<void*>();
        auto szMessage = pException->what();
        if(szMessage)
        {
            ezLog::Dev("%s(%u): Exception in function '%s':",
                       pException->getFileName(),
                       pException->getLineNumber(),
                       pException->getFunctionName());
            ezLog::Dev("  %s", szMessage);
        }
        else
        {
            ezLog::Dev("%s(%u): Exception in function '%s'.",
                       pException->getFileName(),
                       pException->getLineNumber(),
                       pException->getFunctionName());
        }
    }

    if (lcpp::test::detail::g_uiDebugBreakOnExceptions > 0)
    {
        EZ_DEBUG_BREAK;
    }
}

lcpp::Ptr<LispObject> lcpp::test::readStream(StackPtr<LispObject> pStream)
{
    typeCheck(pStream, Type::Stream);

    StackPtr<LispObject> pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    auto pMainStack = cont::getStack(pContMain);

    StackPtr<LispObject> pContRead = cont::create(pContMain, &reader::read);
    cont::getStack(pContRead)->push(pStream);

    cont::trampoline(pContRead);

    return pMainStack->get(0);
}

lcpp::Ptr<LispObject> lcpp::test::readString(const ezString& content)
{
    StackPtr<LispObject> pStream = stream::create(content.GetIteratorFront());

    return readStream(pStream);
}

lcpp::Ptr<LispObject> lcpp::test::evalStream(StackPtr<LispObject> pStream)
{
    StackPtr<LispObject> pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    StackPtr<LispObject> pContEval = cont::create(pContMain, &eval::evaluate);
    cont::getStack(pContEval)->push(LCPP_test_pRuntimeState->getGlobalEnvironment());

    StackPtr<LispObject> pContRead = cont::create(pContEval, &reader::read);
    cont::getStack(pContRead)->push(pStream);

    cont::trampoline(pContRead);

    return cont::getStack(pContMain)->get(-1);
}

lcpp::Ptr<LispObject> lcpp::test::evalString(const ezString& content)
{
    StackPtr<LispObject> pStream = stream::create(content.GetIteratorFront());

    return evalStream(pStream);
}

Ptr<LispObject> lcpp::test::evalObject(StackPtr<LispObject> pObject)
{
    StackPtr<LispObject> pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    StackPtr<LispObject> pContEval = cont::create(pContMain, &eval::evaluate);
    auto pStackEval = cont::getStack(pContEval);
    pStackEval->push(LCPP_test_pRuntimeState->getGlobalEnvironment());
    pStackEval->push(pObject);

    cont::trampoline(pContEval);

    return cont::getStack(pContMain)->get(-1);
}
