#include "stdafx.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/foundation/conversion.h"

lcpp::Ptr<lcpp::LispInteger>
lcpp::LispInteger::create(Number_t value)
{
    EZ_REPORT_FAILURE("Not implemented.");
    return nullptr;
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

ezString
lcpp::LispInteger::toString() const
{
    return lcpp::toString(m_value);
}

//////////////////////////////////////////////////////////////////////////

lcpp::Ptr<lcpp::LispNumber>
lcpp::LispNumber::create(Number_t value)
{
    EZ_REPORT_FAILURE("Not implemented.");
    return nullptr;
}

const lcpp::Type&
lcpp::LispNumber::typeInfo()
{
    static auto t = Type::create(Type::Flags::Arithmetic,
                                 EZ_STRINGIZE(LispNumber),
                                 "integer",
                                 MemoryInfo(sizeof(LispNumber),
                                 EZ_ALIGNMENT_OF(LispNumber)));

    return t;
}

ezString
lcpp::LispNumber::toString() const
{
    return lcpp::toString(m_value);
}
