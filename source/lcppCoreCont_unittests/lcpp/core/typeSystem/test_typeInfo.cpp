#include "stdafx.h"
#include "lcpp/core/typeSystem/typeInfo.h"
#include "lcpp/core/typeSystem/type.h"

LCPP_TEST_GROUP(TypeInfo);

LCPP_TEST(TypeInfo, Basics)
{
    {
        auto info = TypeInfo(Type::Bool);

        CUT_ASSERT.isTrue(info.getType() == Type::Bool);
        CUT_ASSERT.isTrue(info.getPrettyName().IsEqual(info.getType().toString()));
    }

    {
        auto info = TypeInfo(Type::Bool, "boolean");

        CUT_ASSERT.isTrue(info.getType() == Type::Bool);
        CUT_ASSERT.isTrue(info.getPrettyName().IsEqual("boolean"));
    }
}
