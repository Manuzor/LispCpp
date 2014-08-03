#pragma once

namespace lcpp
{
    class Signature
    {
    public:

        enum { VarArg = ezUInt32(-1) };

    public:

        ezUInt32 m_argCountMin;
        ezUInt32 m_argCountMax;
    };
}
