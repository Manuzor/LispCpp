#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"

LCPP_TestGroup(Type);

LCPP_TestCase(Type, Basics)
{
    auto type1 = Type(Type::Nil); ///< Arbitrarily chosen.
    auto type2 = Type(Type::Cons); ///< Arbitrarily chosen.

    CUT_ASSERT.isTrue (type1 == type1);
    CUT_ASSERT.isFalse(type1 == type2);

    CUT_ASSERT.isFalse(type1 != type1);
    CUT_ASSERT.isTrue (type1 != type2);

    // implicit conversion
    CUT_ASSERT.isTrue(type1 == Type::Nil);
    CUT_ASSERT.isFalse(type1 == Type::Cons);

    CUT_ASSERT.isTrue (type2 == Type::Cons);
    CUT_ASSERT.isFalse(type2 == Type::Nil);
}
