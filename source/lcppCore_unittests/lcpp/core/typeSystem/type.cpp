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
        CUT_ASSERT.isTrue(SCHEME_VOID.type() == TypeInfo<LispVoid>::type(), "SCHEME_VOID.type() did not return Type::Void!");
        CUT_ASSERT.isTrue(SCHEME_VOID.is<LispVoid>(), "SCHEME_VOID.is() does not return true for argument Type::Void!");
        CUT_ASSERT.isTrue(SCHEME_TRUE.type() == TypeInfo<LispBool>::type(), "SCHEME_TRUE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(SCHEME_TRUE.is<LispBool>(), "SCHEME_TRUE.is() does not return true for argument Type::Bool!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.type() == TypeInfo<LispBool>::type(), "SCHEME_FALSE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.is<LispBool>(), "SCHEME_FALSE.is() does not return true for argument Type::Bool!");
    });
}
