#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("TypeTests");

    UnitTest g_test1(g_group, "Type", [](){
        CUT_ASSERT.isTrue(SCHEME_VOID.type() == TypeInfo<SchemeVoid>::type(), "SCHEME_VOID.type() did not return Type::Void!");
        CUT_ASSERT.isTrue(SCHEME_VOID.is(TypeInfo<SchemeVoid>::type()), "SCHEME_VOID.is() does not return true for argument Type::Void!");
        CUT_ASSERT.isTrue(SCHEME_TRUE.type() == TypeInfo<SchemeBool>::type(), "SCHEME_TRUE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(SCHEME_TRUE.is(TypeInfo<SchemeBool>::type()), "SCHEME_TRUE.is() does not return true for argument Type::Bool!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.type() == TypeInfo<SchemeBool>::type(), "SCHEME_FALSE.type() did not return Type::Bool!");
        CUT_ASSERT.isTrue(SCHEME_FALSE.is(TypeInfo<SchemeBool>::type()), "SCHEME_FALSE.is() does not return true for argument Type::Bool!");
    });
}
