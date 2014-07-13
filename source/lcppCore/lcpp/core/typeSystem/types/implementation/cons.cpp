#include "stdafx.h"
#include "lcpp/core/typeSystem/types/cons.h"

lcpp::Ptr<lcpp::LispCons>
lcpp::LispCons::create(Ptr<LispObject> pCar, Ptr<LispObject> pCdr)
{
    EZ_REPORT_FAILURE("Not implemented.");
    return nullptr;
}

const lcpp::Type&
lcpp::LispCons::typeInfo()
{
    static auto t = Type::create(EZ_STRINGIZE(LispCons),
                                 "cons",
                                 MemoryInfo(sizeof(LispCons),
                                 EZ_ALIGNMENT_OF(LispCons)));

    return t;
}
