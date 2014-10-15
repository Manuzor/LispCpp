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

lcpp::test::UnitTestNoInit::UnitTestNoInit(cut::UnitTestGroup& group)
{
    group.registerUnitTest(this);
}

lcpp::Ptr<LispObject> lcpp::test::readStream(Ptr<LispObject> pStream)
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

lcpp::Ptr<LispObject> lcpp::test::evalStream(Ptr<LispObject> pStream)
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

Ptr<LispObject> lcpp::test::evalObject(Ptr<LispObject> pObject)
{
    StackPtr<LispObject> pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
    StackPtr<LispObject> pContEval = cont::create(pContMain, &eval::evaluate);
    auto pStackEval = cont::getStack(pContEval);
    pStackEval->push(LCPP_test_pRuntimeState->getGlobalEnvironment());
    pStackEval->push(pObject);

    cont::trampoline(pContEval);

    return cont::getStack(pContMain)->get(-1);
}
