#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"

LCPP_TestGroup(MetaInfo);

LCPP_TestCase(MetaInfo, Basics)
{
    {
        auto info = MetaInfo(Type::True);

        CUT_ASSERT.isTrue(info.getType() == Type::True);
        CUT_ASSERT.isTrue(info.getPrettyName().IsEqual(info.getType().toString()));
    }

    {
        auto info = MetaInfo(Type::True, "true-type");

        CUT_ASSERT.isTrue(info.getType() == Type::True);
        CUT_ASSERT.isTrue(info.getPrettyName().IsEqual("true-type"));
    }
}
