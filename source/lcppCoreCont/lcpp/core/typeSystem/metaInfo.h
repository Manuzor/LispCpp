#pragma once

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"

namespace lcpp
{
    class MetaPropertyId
    {
    public:

        EZ_DECLARE_POD_TYPE();

        MetaPropertyId();

        MetaPropertyId(ezUInt32 value);

        ezUInt32 getValue() const;

    private:
        ezUInt32 m_value;
    };

    bool operator ==(const MetaPropertyId& lhs, const MetaPropertyId& rhs);
    bool operator <(const MetaPropertyId& lhs, const MetaPropertyId& rhs);
    bool operator <=(const MetaPropertyId& lhs, const MetaPropertyId& rhs);

}

template<>
struct ezHashHelper<lcpp::MetaPropertyId>
{
    static ezUInt32 Hash(const lcpp::MetaPropertyId& value);
    static bool Equal(const lcpp::MetaPropertyId& lhs, const lcpp::MetaPropertyId& rhs);
};

namespace lcpp
{

    class MetaPropertyData
    {
    public:

        MetaPropertyData();

        /// \brief \a sizeof(T_Data) must be <= sizeof(void*).
        template<typename T_Data>
        MetaPropertyData(const T_Data& data);

        template<typename T_Data>
        T_Data& as();

        template<typename T_Data>
        const T_Data& as() const;

    private:

        typedef void* RawData_t;

    private:

        RawData_t m_rawData;
    };

    class MetaProperty
    {
    public:

        EZ_DECLARE_POD_TYPE();

        class Builtin
        {
        public:

            enum Enum
            {
                CallFunction,
                ToStringFunction,
                ScanFunction,
                DestructorFunction,
                IsEqualFunction,

                // Since property IDs are simply integers, you can easily define your own properties.
                // When you do, start at id 64 or higher.
                CAPACITY = 64,
            };
        };

    public:

        MetaProperty();

        MetaProperty(MetaPropertyId id, MetaPropertyData data);

        MetaPropertyId getId() const;

        MetaPropertyData getData() const;

    private:

        MetaPropertyId m_id;
        MetaPropertyData m_data;
    };

    /// \brief This class is meant to have a unique, immutable instance for every needed type in the system.
    class MetaInfo
    {
    public:

        MetaInfo();

        void setType(const Type& type);
        const Type& getType() const;

        void setAttributes(const AttributeFlags& attributes);
        const AttributeFlags& getAttributes() const;

        void setPrettyName(const char* szPrettyName);
        const char* getPrettyName() const;

        void addProperty(const MetaProperty& prop);
        ezResult getProperty(const MetaPropertyId& id, MetaProperty& out_prop) const;

    public:

        typedef ezHybridArray<MetaProperty, 16, AllocatorWrapper_Default> PropertyTable_t;

    private:

        Type m_type;
        AttributeFlags m_attributes;

        String m_prettyName;

        PropertyTable_t m_properties;
    };
}

#include "lcpp/core/typeSystem/impl/metaInfo.inl"
