#include "stdafx.h"
#include "lcpp/core/typeSystem/types/string.h"

lcpp::Ptr<lcpp::LispString>
lcpp::LispString::create(const ezString& value)
{
    EZ_REPORT_FAILURE("Not implemeneted.");
    return nullptr;
}

const lcpp::Type&
lcpp::LispString::typeInfo()
{
    static auto t = Type::create(Type::Flags::None,
                                 EZ_STRINGIZE(LispString),
                                 "function (base)",
                                 MemoryInfo(sizeof(LispString),
                                 sizeof(LispString)));

    return t;
}
