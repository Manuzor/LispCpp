#pragma once

namespace lcpp
{
    class RefIndex
    {
    public:

        static RefIndex invalidValue();

    public:

        ezUInt32 m_uiIndex;
    };

    EZ_CHECK_AT_COMPILETIME(sizeof(RefIndex) == sizeof(ezUInt32));

    bool operator == (const RefIndex& lhs, const RefIndex& rhs);
}

#include "lcpp/core/memory/impl/refIndex.inl"
