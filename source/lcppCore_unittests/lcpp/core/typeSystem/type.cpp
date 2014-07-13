#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("TypeTests");

    UnitTest g_test1(g_group, "InstanceIdentity", []{
        CUT_ASSERT.isTrue(TypeInfo<LispInteger>::type() == TypeInfo<LispInteger>::type());
        CUT_ASSERT.isTrue(TypeInfo<LispInteger>::type() == TypeInfo<LispInteger>::type());

        CUT_ASSERT.isTrue(TypeInfo<LispNumber>::type() == TypeInfo<LispNumber>::type());
        CUT_ASSERT.isTrue(TypeInfo<LispNumber>::type() == TypeInfo<LispNumber>::type());

        CUT_ASSERT.isTrue(TypeInfo<LispNumber>::type() != TypeInfo<LispInteger>::type());
        CUT_ASSERT.isTrue(TypeInfo<LispInteger>::type() != TypeInfo<LispNumber>::type());

        CUT_ASSERT.isTrue(TypeInfo<LispNil>::type() == TypeInfo<LispNil>::type());
        CUT_ASSERT.isTrue(TypeInfo<LispNil>::type() != TypeInfo<LispVoid>::type());

        auto& first = TypeInfo<LispInteger>::type();
        auto& second = TypeInfo<LispInteger>::type();
        auto& third = TypeInfo<LispNumber>::type();

        CUT_ASSERT.isTrue(&first == &second);
        CUT_ASSERT.isTrue(&first != &third);
        CUT_ASSERT.isTrue(first.id != third.id);
    });

    UnitTest g_test2(g_group, "SingletonTypes", []{
        CUT_ASSERT.isTrue(LCPP_VOID->type() == TypeInfo<LispVoid>::type(), "LCPP_VOID.type() did not return Type::Void!");
        CUT_ASSERT.isTrue(LCPP_VOID->is<LispVoid>(), "LCPP_VOID.is() does not return true for argument Type::Void!");
        CUT_ASSERT.isTrue(LCPP_TRUE->type() == TypeInfo<LispBool>::type(), "LCPP_TRUE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(LCPP_TRUE->is<LispBool>(), "LCPP_TRUE.is() does not return true for argument Type::Bool!");
        CUT_ASSERT.isTrue(LCPP_FALSE->type() == TypeInfo<LispBool>::type(), "LCPP_FALSE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(LCPP_FALSE->is<LispBool>(), "LCPP_FALSE.is() does not return true for argument Type::Bool!");
    });
}
