#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/exceptions/exceptions.h"

lcpp::Ptr<lcpp::LispObject>
lcpp::LispObject::create()
{
    static auto szMessage = "Cannot directly instantiate LispObject";
    EZ_REPORT_FAILURE(szMessage);
    throw exceptions::InvalidOperation(szMessage);
}

const lcpp::Type&
lcpp::LispObject::typeInfo()
{
    static auto t = Type::create(Type::Flags::Abstract,
                                 EZ_STRINGIZE(LispObject),
                                 "object",
                                 MemoryInfo(sizeof(LispObject),
                                            sizeof(LispObject)));

    return t;
}
