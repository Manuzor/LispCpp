#pragma once

namespace lcpp
{
    class StandardOutputStream : public ezStreamWriterBase
    {
    public:
        StandardOutputStream();
        virtual ezResult WriteBytes(const void* pWriteBuffer, ezUInt64 uiBytesToWrite) override;
        virtual ezResult Flush() override;
    };
}

#include "lcpp/core/impl/ioUtils.inl"
