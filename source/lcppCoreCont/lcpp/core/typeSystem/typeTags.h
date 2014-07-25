#pragma once

namespace lcpp
{
    class TypeTags
    {
    public:

        enum Enum
        {
            None = 0,
            Callable = EZ_BIT(0),
            Nameable = EZ_BIT(1),
        };

    public:

        TypeTags();

        TypeTags(ezUInt32 flags);

        bool isCallable() const;
        bool isNameable() const;

    private:
        ezUInt32 m_flags;
    };
}

#include "lcpp/core/typeSystem/impl/typeTags.inl"
