#pragma once

namespace lcpp
{
    class RefIndex
    {
    public:

        static RefIndex invalidValue();

    public:

        bool isValid() const;

    public:

        union
        {
            struct
            {
                ezUInt32 m_uiHash : 8;
                ezUInt32 m_uiIndex : 24;
            };
            ezUInt32 m_uiId;
        };
    };

    EZ_CHECK_AT_COMPILETIME(sizeof(RefIndex) == sizeof(ezUInt32));

    bool operator == (const RefIndex& lhs, const RefIndex& rhs);
    bool operator != (const RefIndex& lhs, const RefIndex& rhs);
}

#include "lcpp/core/memory/impl/refIndex.inl"
