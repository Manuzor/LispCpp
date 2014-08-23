#pragma once

namespace lcpp
{
    class MetaInfo;

    class LispObjectHeader
    {
    public:

        Ptr<const MetaInfo> m_pMetaInfo;
    };

    typedef void* LispObjectTypeData;

    class LCPP_API_CORE_CONT LispObject
    {
    public:

        LispObjectHeader m_header;

    private:

        /// \brief This is supposed to be reinterpreted to the desired type, such as cons::Data, env::Data, etc.
        /// \remark Use getData<env::Data>() for example to treat this object as an environment and access its data.
        LispObjectTypeData m_typeSpecificData;

    public:

        LispObject(const MetaInfo& metaInfo);

        template<typename T_Data>
        T_Data& getData();
    };
}

#include "lcpp/core/typeSystem/impl/objectData.inl"
