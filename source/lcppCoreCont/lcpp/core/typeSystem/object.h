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

    class LispObjectBody
    {
    public:

        union
        {
            number::Data m_number;
        };
    };

    class LCPP_API_CORE_CONT LispObject
    {
    public:

        template<typename T_Data>
        static Ptr<LispObject> create(const MetaInfo& metaInfo);

    public:

        LispObject(const MetaInfo& metaInfo);

        bool isType(const Type& type) const;
        const Type& getType() const;

        const MetaInfo& getMetaInfo() const;

        const LispObjectHeader& getHeader() const;

        const LispObjectBody& getBody() const;
        LispObjectBody& getBody();

    private:

        LispObjectHeader m_header;

        LispObjectBody m_body;

    };

    //static_assert(sizeof(LispObject) == sizeof(LispObjectHeader)
    //                                  + sizeof(LispObjectBody),
    //              "Add some padding so the alignment is perfect again");
}

#include "lcpp/core/typeSystem/impl/object.inl"
