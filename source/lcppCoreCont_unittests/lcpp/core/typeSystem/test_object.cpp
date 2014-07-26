#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

LCPP_TestGroup(Object);

LCPP_TestCase(Object, Basics)
{
    auto pObject = LCPP_pNil;

    CUT_ASSERT.isTrue(pObject->isType(Type::Nil));
    CUT_ASSERT.isTrue(pObject->getType() == Type(Type::Nil));
    CUT_ASSERT.isTrue(pObject->getMetaInfo().getType() == pObject->getType());
}
