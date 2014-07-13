#include "stdafx.h"
#include "lcpp/core/typeSystem/types/symbol.h"


lcpp::Ptr<lcpp::LispSymbol>
lcpp::LispSymbol::create(const ezString& symbol)
{
    EZ_REPORT_FAILURE("Not implemented.");
    return nullptr;
}

const lcpp::Type&
lcpp::LispSymbol::typeInfo()
{
    static auto t = Type::create(EZ_STRINGIZE(LispSymbol),
                                 "symbol",
                                 MemoryInfo(sizeof(LispSymbol),
                                 sizeof(LispSymbol)));

    return t;
}
