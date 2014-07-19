#include "stdafx.h"
#include "lcpp/core/typeSystem/baseTypes/nameable.h"
#include "lcpp/core/typeSystem/type.h"

const lcpp::Type&
lcpp::NameableBase::typeInfo()
{
    static auto t = Type::create(Type::Flags::Nameable,
                                 EZ_STRINGIZE(NameableBase),
                                 "nameable",
                                 MemoryInfo::create<NameableBase>());
    return t;
}
