#pragma once

namespace lcpp
{
    class RefIndex
    {
    public:

        static RefIndex invalidValue();

    public:

        bool isValid() const;
        bool isStatic() const;

    public:

        union
        {
            struct
            {
                ezUInt64 m_bIsStatic : 1;
                ezUInt64 m_uiHash : 1;
                ezUInt64 m_uiIndex : 62;
            };
            ezUInt64 m_uiId;
        };
    };

    EZ_CHECK_AT_COMPILETIME(sizeof(RefIndex) == sizeof(ezUInt64));

    bool operator == (const RefIndex& lhs, const RefIndex& rhs);
    bool operator != (const RefIndex& lhs, const RefIndex& rhs);
}

#include "lcpp/core/memory/impl/refIndex.inl"
