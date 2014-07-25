#include "stdafx.h"
#include "lcpp/core/typeSystem/typeInfo.h"
#include "lcpp/core/typeSystem/type.h"

LCPP_TEST_GROUP(TypeSystem);

LCPP_TEST(TypeSystem, Type)
{
    auto type1 = Type(Type::Bool); ///< Arbitrarily chosen.
    auto type2 = Type(Type::Cons); ///< Arbitrarily chosen.

    CUT_ASSERT.isTrue (type1 == type1);
    CUT_ASSERT.isFalse(type1 == type2);

    CUT_ASSERT.isFalse(type1 != type1);
    CUT_ASSERT.isTrue (type1 != type2);

    // implicit conversion
    CUT_ASSERT.isTrue (type1 == Type::Bool);
    CUT_ASSERT.isFalse(type1 == Type::Cons);

    CUT_ASSERT.isTrue (type2 == Type::Cons);
    CUT_ASSERT.isFalse(type2 == Type::Bool);
}

LCPP_TEST(TypeSystem, TypeInfo)
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
