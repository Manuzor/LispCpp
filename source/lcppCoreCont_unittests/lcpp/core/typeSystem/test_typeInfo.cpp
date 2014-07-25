#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"

LCPP_TEST_GROUP(MetaInfo);

LCPP_TEST(MetaInfo, Basics)
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
