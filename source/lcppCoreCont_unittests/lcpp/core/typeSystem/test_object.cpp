#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

LCPP_TEST_GROUP(Object);

LCPP_TEST(Object, Basics)
{
    auto pObject = LCPP_NIL;

    CUT_ASSERT.isTrue(pObject->isType(Type::Nil));
    CUT_ASSERT.isTrue(pObject->getType() == Type(Type::Nil));
    CUT_ASSERT.isTrue(pObject->getMetaInfo().getType() == pObject->getType());
}
