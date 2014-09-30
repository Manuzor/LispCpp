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
    LCPP_test_pRuntimeState->initialize();
}

void lcpp::test::UnitTest::shutdown()
{
    // TODO shutdown stuff here on a per-test basis
}

lcpp::StackPtr<LispObject> lcpp::test::readStream(StackPtr<LispObject> pStream)
{
    typeCheck(pStream, Type::Stream);

    auto pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    auto pMainStack = cont::getStack(pContMain);

    auto pContRead = cont::create(pContMain, &reader::read);
    cont::getStack(pContRead)->push(pStream);

    cont::trampoline(pContRead);

    return pMainStack->get(0);
}

lcpp::StackPtr<LispObject> lcpp::test::readString(const ezString& content)
{
    auto pStream = stream::create(content.GetIteratorFront());

    return readStream(pStream);
}

lcpp::StackPtr<LispObject> lcpp::test::evalStream(StackPtr<LispObject> pStream)
{
    auto pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    auto pContEval = cont::create(pContMain, &eval::evaluate);
    cont::getStack(pContEval)->push(LCPP_test_pRuntimeState->getGlobalEnvironment());

    auto pContRead = cont::create(pContEval, &reader::read);
    cont::getStack(pContRead)->push(pStream);

    cont::trampoline(pContRead);

    return cont::getStack(pContMain)->get(-1);
}

lcpp::StackPtr<LispObject> lcpp::test::evalString(const ezString& content)
{
    auto pStream = stream::create(content.GetIteratorFront());

    return evalStream(pStream);
}

StackPtr<LispObject> lcpp::test::evalObject(StackPtr<LispObject> pObject)
{
    auto pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    auto pContEval = cont::create(pContMain, &eval::evaluate);
    auto pStackEval = cont::getStack(pContEval);
    pStackEval->push(LCPP_test_pRuntimeState->getGlobalEnvironment());
    pStackEval->push(pObject);

    cont::trampoline(pContEval);

    return cont::getStack(pContMain)->get(-1);
}
