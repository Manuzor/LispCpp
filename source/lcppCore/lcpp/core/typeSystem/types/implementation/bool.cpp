#include "stdafx.h"
#include "lcpp/core/typeSystem/types/bool.h"

lcpp::Ptr<lcpp::LispBool>
lcpp::LispBool::create(bool value)
{
    static auto trueInstance = LispBool(true);
    static auto pTrue = Ptr<LispBool>(&trueInstance);
    static auto falseInstance = LispBool(false);
    static auto pFalse = Ptr<LispBool>(&falseInstance);

    return value ? pTrue : pFalse;
}

const lcpp::Type&
lcpp::LispBool::typeInfo()
{
    static auto instance = Type::create(Type::Flags::Singleton,
                                        EZ_STRINGIZE(LispBool),
                                        "bool",
                                        MemoryInfo(sizeof(LispBool),
                                        sizeof(LispBool)));

    return instance;
}
