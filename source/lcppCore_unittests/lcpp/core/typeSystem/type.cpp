#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("TypeTests");

    UnitTest g_test1(g_group, "InstanceIdentity", []{
        CUT_ASSERT.isTrue(LispInteger::typeInfo() == LispInteger::typeInfo());
        CUT_ASSERT.isTrue(LispInteger::typeInfo() == LispInteger::typeInfo());

        CUT_ASSERT.isTrue(LispNumber::typeInfo() == LispNumber::typeInfo());
        CUT_ASSERT.isTrue(LispNumber::typeInfo() == LispNumber::typeInfo());

        CUT_ASSERT.isTrue(LispNumber::typeInfo() != LispInteger::typeInfo());
        CUT_ASSERT.isTrue(LispInteger::typeInfo() != LispNumber::typeInfo());

        CUT_ASSERT.isTrue(LispNil::typeInfo() == LispNil::typeInfo());
        CUT_ASSERT.isTrue(LispNil::typeInfo() != LispVoid::typeInfo());

        auto& first = LispInteger::typeInfo();
        auto& second = LispInteger::typeInfo();
        auto& third = LispNumber::typeInfo();

        CUT_ASSERT.isTrue(&first == &second);
        CUT_ASSERT.isTrue(&first != &third);
        CUT_ASSERT.isTrue(first.id != third.id);
    });

    UnitTest g_test2(g_group, "SingletonTypes", []{
        CUT_ASSERT.isTrue(LCPP_VOID->type() == LispVoid::typeInfo(), "LCPP_VOID.type() did not return Type::Void!");
        CUT_ASSERT.isTrue(LCPP_VOID->is<LispVoid>(), "LCPP_VOID.is() does not return true for argument Type::Void!");
        CUT_ASSERT.isTrue(LCPP_TRUE->type() == LispBool::typeInfo(), "LCPP_TRUE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(LCPP_TRUE->is<LispBool>(), "LCPP_TRUE.is() does not return true for argument Type::Bool!");
        CUT_ASSERT.isTrue(LCPP_FALSE->type() == LispBool::typeInfo(), "LCPP_FALSE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(LCPP_FALSE->is<LispBool>(), "LCPP_FALSE.is() does not return true for argument Type::Bool!");
    });
}
