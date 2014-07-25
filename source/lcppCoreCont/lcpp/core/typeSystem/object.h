#pragma once

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/metaInfo.h"

#include "lcpp/core/typeSystem/includeAllTypes.h"

namespace lcpp
{
    class LispObjectHeader
    {
    public:

        Ptr<const MetaInfo> m_pMetaInfo;

    };

    class LCPP_API_CORE_CONT LispObject
    {
    public:

        LispObject(const MetaInfo& metaInfo);

        bool isType(const Type& type) const;
        const Type& getType() const;

        const MetaInfo& getMetaInfo() const;

    private:

        LispObjectHeader m_header;

    public:

        union
        {
            number::Data m_number;
        };
    };
}

#include "lcpp/core/typeSystem/impl/object.inl"
