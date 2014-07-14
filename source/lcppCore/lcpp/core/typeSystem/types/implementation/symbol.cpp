#include "stdafx.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/runtime.h"


lcpp::Ptr<lcpp::LispSymbol>
lcpp::LispSymbol::create(const ezString& symbol)
{
    auto pRuntime = LispRuntime::instance();
    auto instanceTables = pRuntime->instanceTables();
    auto pSymbolTable = instanceTables.pSymbols;
    return pSymbolTable->get(symbol);
}

lcpp::Ptr<lcpp::LispSymbol>
lcpp::LispSymbol::createNew(const ezString& value)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    return LCPP_NEW(pAllocator, LispSymbol)(value);
}

const lcpp::Type&
lcpp::LispSymbol::typeInfo()
{
    static auto t = Type::create(Type::Flags::None,
                                 EZ_STRINGIZE(LispSymbol),
                                 "symbol",
                                 MemoryInfo(sizeof(LispSymbol),
                                 sizeof(LispSymbol)));

    return t;
}
