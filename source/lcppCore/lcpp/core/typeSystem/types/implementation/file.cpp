#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/file.h"
#include "lcpp/core/runtime.h"

lcpp::Ptr<lcpp::LispFile>
lcpp::LispFile::create(const String& fileName)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    return LCPP_NEW(pAllocator, LispFile)(fileName);
}

lcpp::Ptr<lcpp::LispFile>
lcpp::LispFile::copy(const LispFile& toCopy)
{
    auto pRuntime = LispRuntime::instance();
    auto pAllocator = pRuntime->allocator().get();
    return LCPP_NEW(pAllocator, LispFile)(toCopy);
}

const lcpp::Type&
lcpp::LispFile::typeInfo()
{
    static auto t = Type::create(Type::Flags::None,
                                 EZ_STRINGIZE(LispFile),
                                 "file",
                                 MemoryInfo(sizeof(LispFile),
                                 EZ_ALIGNMENT_OF(LispFile)));

    return t;
}
