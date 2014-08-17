#pragma once

namespace lcpp
{
    ezResult findDirectory(const char* szDirName, const char* szStartPath, String* out_pParentPath);

    ezStreamWriterBase& operator << (ezStreamWriterBase& Stream, const ezStringBuilder& builder);
}

#include "lcpp/cli/implementation/ioUtils.inl"
