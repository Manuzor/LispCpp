#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/file.h"

lcpp::Ptr<lcpp::LispFile>
lcpp::LispFile::create(const ezString& fileName)
{
    EZ_REPORT_FAILURE("Not implemented.");
    return nullptr;
}

const lcpp::Type&
lcpp::LispFile::typeInfo()
{
    static auto t = Type::create(EZ_STRINGIZE(LispFile),
                                 "file",
                                 MemoryInfo(sizeof(LispFile),
                                 EZ_ALIGNMENT_OF(LispFile)));

    return t;
}
