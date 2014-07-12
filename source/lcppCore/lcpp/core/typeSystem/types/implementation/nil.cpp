#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"

lcpp::Ptr<lcpp::LispNil>
lcpp::LispNil::create()
{
    static LispNil nil;
    return &nil;
}

const lcpp::Type& lcpp::LispNil::typeInfo()
{
    static auto t = Type::create(EZ_STRINGIZE(LispNil),
                                 "Nil",
                                 MemoryInfo(sizeof(LispNil),
                                            sizeof(LispNil)));

    return t;
}
