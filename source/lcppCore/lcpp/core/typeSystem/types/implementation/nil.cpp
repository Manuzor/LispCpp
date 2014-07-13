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
    static auto t = Type::create(Type::Flags::Singleton,
                                 EZ_STRINGIZE(LispNil),
                                 "nil",
                                 MemoryInfo(sizeof(LispNil),
                                            EZ_ALIGNMENT_OF(LispNil)));

    return t;
}
