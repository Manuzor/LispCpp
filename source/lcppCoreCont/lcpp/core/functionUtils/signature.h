#pragma once

namespace lcpp
{
    class Signature
    {
    public:

        enum { VarArg = ezUInt32(-1) };

    public:

        static Signature create(ezUInt32 argMin, ezUInt32 argMax);
        static Signature create(ezUInt32 argCount);
        static Signature createVarArg(ezUInt32 argMin = 0);

    public:

        ezUInt32 m_argCountMin;
        ezUInt32 m_argCountMax;
    };
}

#include "lcpp/core/functionUtils/impl/signature.inl"
