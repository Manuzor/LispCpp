#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("TypeTests");

    UnitTest g_test1(g_group, "InstanceIdentity", []{
        CUT_ASSERT.isTrue(TypeInfo<SchemeInteger>::type() == TypeInfo<SchemeInteger>::type());
        CUT_ASSERT.isTrue(TypeInfo<SchemeInteger>::type() == TypeInfo<SchemeInteger>::type());

        CUT_ASSERT.isTrue(TypeInfo<SchemeNumber>::type() == TypeInfo<SchemeNumber>::type());
        CUT_ASSERT.isTrue(TypeInfo<SchemeNumber>::type() == TypeInfo<SchemeNumber>::type());

        CUT_ASSERT.isTrue(TypeInfo<SchemeNumber>::type() != TypeInfo<SchemeInteger>::type());
        CUT_ASSERT.isTrue(TypeInfo<SchemeInteger>::type() != TypeInfo<SchemeNumber>::type());

        CUT_ASSERT.isTrue(TypeInfo<SchemeNil>::type() == TypeInfo<SchemeNil>::type());
        CUT_ASSERT.isTrue(TypeInfo<SchemeNil>::type() != TypeInfo<SchemeVoid>::type());

        auto& first = TypeInfo<SchemeInteger>::type();
        auto& second = TypeInfo<SchemeInteger>::type();
        auto& third = TypeInfo<SchemeNumber>::type();

        CUT_ASSERT.isTrue(&first == &second);
        CUT_ASSERT.isTrue(&first != &third);
        CUT_ASSERT.isTrue(first.id != third.id);
    });

    UnitTest g_test2(g_group, "SingletonTypes", []{
        CUT_ASSERT.isTrue(SCHEME_VOID.type() == TypeInfo<SchemeVoid>::type(), "SCHEME_VOID.type() did not return Type::Void!");
        CUT_ASSERT.isTrue(SCHEME_VOID.is<SchemeVoid>(), "SCHEME_VOID.is() does not return true for argument Type::Void!");
        CUT_ASSERT.isTrue(SCHEME_TRUE.type() == TypeInfo<SchemeBool>::type(), "SCHEME_TRUE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(SCHEME_TRUE.is<SchemeBool>(), "SCHEME_TRUE.is() does not return true for argument Type::Bool!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.type() == TypeInfo<SchemeBool>::type(), "SCHEME_FALSE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.is<SchemeBool>(), "SCHEME_FALSE.is() does not return true for argument Type::Bool!");
    });
}
