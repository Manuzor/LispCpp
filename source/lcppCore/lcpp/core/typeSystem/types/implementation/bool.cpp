#include "stdafx.h"
#include "lcpp/core/typeSystem/types/bool.h"

lcpp::Ptr<lcpp::LispBool>
lcpp::LispBool::trueInstance()
{
    static auto instance = LispBool(true);
    static auto pTrue = Ptr<LispBool>(&instance);
    return pTrue;
}

lcpp::Ptr<lcpp::LispBool>
lcpp::LispBool::falseInstance()
{
    static auto instance = LispBool(false);
    static auto pFalse = Ptr<LispBool>(&instance);
    return pFalse;
}

const lcpp::Type&
lcpp::LispBool::typeInfo()
{
    static auto instance = Type::create(EZ_STRINGIZE(LispBool),
                                        "bool",
                                        MemoryInfo(sizeof(LispBool),
                                        sizeof(LispBool)));

    return instance;
}
