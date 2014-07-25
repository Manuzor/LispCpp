#pragma once

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/metaInfo.h"

#include "lcpp/core/typeSystem/includeAllTypes.h"

namespace lcpp
{
    class LCPP_API_CORE_CONT LispObject
    {
    public:

        LispObject(const MetaInfo& metaInfo);

        ~LispObject();

        bool isType(const Type& type) const;
        const Type& getType() const;

        const MetaInfo& getMetaInfo() const;

    private:

        const MetaInfo* m_pMetaInfo;

        union
        {
        };

    private:

    };
}

#include "lcpp/core/typeSystem/impl/object.inl"
