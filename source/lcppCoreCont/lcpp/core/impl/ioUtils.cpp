#include "stdafx.h"
#include "lcpp/core/ioUtils.h"
#include <iostream>

namespace lcpp
{
    StandardOutputStream::StandardOutputStream()
    {
    }

    ezResult StandardOutputStream::WriteBytes(const void* pWriteBuffer, ezUInt64 uiBytesToWrite)
    {
        std::cout.write((const char*)pWriteBuffer, uiBytesToWrite);
        return EZ_SUCCESS;
    }

    ezResult StandardOutputStream::Flush()
    {
        std::cout.flush();
        return EZ_SUCCESS;
    }
}
