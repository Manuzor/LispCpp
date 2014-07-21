#include "stdafx.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/runtime.h"

lcpp::Ptr<lcpp::LispString>
lcpp::LispString::create(const ezString& value)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    return LCPP_NEW(pAllocator, LispString)(value);
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

lcpp::LispString::LispString(const char* str) :
    m_string(LispRuntime::instance()->allocator().get())
{
    m_string = str;
}

lcpp::LispString::LispString(const ezString& str) :
    m_string(LispRuntime::instance()->allocator().get())
{
    m_string = str;
}
