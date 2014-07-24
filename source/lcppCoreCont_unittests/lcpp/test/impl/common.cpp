#include "stdafx.h"
#include "lcpp/test/common.h"

using namespace lcpp::test;

UnitTest::UnitTest(cut::UnitTestGroup& group)
{
    group.registerUnitTest(this);
}

void UnitTest::inititialize()
{
    // TODO init stuff here on a per-test basis
}

void UnitTest::shutdown()
{
    // TODO shutdown stuff here on a per-test basis
}
