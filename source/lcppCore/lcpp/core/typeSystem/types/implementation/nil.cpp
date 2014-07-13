#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/cons.h"

lcpp::Ptr<lcpp::LispNil>
lcpp::LispNil::create()
{
    static auto instance = LispNil();
    static auto pNil = Ptr<LispNil>(&instance);
    return pNil;
}

const lcpp::Type&
lcpp::LispNil::typeInfo()
{
    static auto t = Type::create(EZ_STRINGIZE(LispNil),
                                 "nil",
                                 MemoryInfo(sizeof(LispNil),
                                            sizeof(LispNil)));

    return t;
}
