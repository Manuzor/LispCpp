#include "stdafx.h"
#include "lcpp/test/common.h"
#include "lcpp/core/runtime.h"

using namespace lcpp::test;

UnitTest::UnitTest(cut::UnitTestGroup& group)
{
    group.registerUnitTest(this);
}

void UnitTest::inititialize()
{
    LCPP_test_pRuntimeState->initialize();
}

void UnitTest::shutdown()
{
    // TODO shutdown stuff here on a per-test basis
}
