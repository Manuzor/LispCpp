#include "stdafx.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/foundation/conversion.h"
#include "lcpp/core/runtime.h"

lcpp::Ptr<lcpp::LispInteger>
lcpp::LispInteger::create(Number_t value)
{
    auto pRuntime = LispRuntime::instance();
    auto instanceTables = pRuntime->instanceTables();
    auto pIntegerTable = instanceTables.pIntegers;
    return pIntegerTable->get(value);
}

lcpp::Ptr<lcpp::LispInteger>
lcpp::LispInteger::createNew(Number_t value)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    return LCPP_NEW(pAllocator, LispInteger)(value);
}

const lcpp::Type&
lcpp::LispInteger::typeInfo()
{
    static auto t = Type::create(Type::Flags::Arithmetic,
                                 EZ_STRINGIZE(LispInteger),
                                 "integer",
                                 MemoryInfo(sizeof(LispInteger),
                                            EZ_ALIGNMENT_OF(LispInteger)));

    return t;
}

lcpp::String
lcpp::LispInteger::toString() const
{
    return lcpp::toString(m_value);
}

//////////////////////////////////////////////////////////////////////////

lcpp::Ptr<lcpp::LispNumber>
lcpp::LispNumber::create(Number_t value)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    return LCPP_NEW(pAllocator, LispNumber)(value);
}

const lcpp::Type&
lcpp::LispNumber::typeInfo()
{
    static auto t = Type::create(Type::Flags::Arithmetic,
                                 EZ_STRINGIZE(LispNumber),
                                 "number",
                                 MemoryInfo(sizeof(LispNumber),
                                            EZ_ALIGNMENT_OF(LispNumber)));

    return t;
}

lcpp::String
lcpp::LispNumber::toString() const
{
    return lcpp::toString(m_value);
}
