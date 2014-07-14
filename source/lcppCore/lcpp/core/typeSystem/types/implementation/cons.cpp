#include "stdafx.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/runtime.h"

lcpp::Ptr<lcpp::LispCons>
lcpp::LispCons::create(Ptr<LispObject> pCar, Ptr<LispObject> pCdr)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    return LCPP_NEW(pAllocator, LispCons)(pCar, pCdr);
}

const lcpp::Type&
lcpp::LispCons::typeInfo()
{
    static auto t = Type::create(Type::Flags::None,
                                 EZ_STRINGIZE(LispCons),
                                 "cons",
                                 MemoryInfo(sizeof(LispCons),
                                            EZ_ALIGNMENT_OF(LispCons)));

    return t;
}
