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
            number::Integer_t m_integer;
            number::Float_t m_float;
            symbol::Data m_symbol;
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
}

#include "lcpp/core/typeSystem/impl/object.inl"
