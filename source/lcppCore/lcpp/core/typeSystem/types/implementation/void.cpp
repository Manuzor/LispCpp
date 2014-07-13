#include "stdafx.h"
#include "lcpp/core/typeSystem/types/void.h"


lcpp::Ptr<lcpp::LispVoid>
lcpp::LispVoid::create()
{
    static auto instance = LispVoid();
    static auto pVoid = Ptr<LispVoid>(&instance);
    return pVoid;
}

const lcpp::Type&
lcpp::LispVoid::typeInfo()
{
    static auto instance = Type::create(Type::Flags::Singleton,
                                        EZ_STRINGIZE(LispVoid),
                                        "void",
                                        MemoryInfo(sizeof(LispVoid),
                                                   sizeof(LispVoid)));

    return instance;
}
