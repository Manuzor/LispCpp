
namespace lcpp
{
    EZ_FORCE_INLINE
    MetaPropertyId::MetaPropertyId() :
        m_value(0)
    {
    }

    EZ_FORCE_INLINE
    MetaPropertyId::MetaPropertyId(ezUInt32 value) :
        m_value(value)
    {
    }

    EZ_FORCE_INLINE
    ezUInt32 MetaPropertyId::getValue() const
    {
        return m_value;
    }

    EZ_FORCE_INLINE
    bool operator==(const MetaPropertyId& lhs, const MetaPropertyId& rhs)
    {
        return lhs.getValue() == rhs.getValue();
    }
}

EZ_FORCE_INLINE
ezUInt32 ezHashHelper<lcpp::MetaPropertyId>::Hash(const lcpp::MetaPropertyId& value)
{
    return ezHashHelper<decltype(value.getValue())>::Hash(value.getValue());
}

EZ_FORCE_INLINE
bool ezHashHelper<lcpp::MetaPropertyId>::Equal(const lcpp::MetaPropertyId& lhs, const lcpp::MetaPropertyId& rhs)
{
    using namespace lcpp;
    return lhs == rhs;
}

namespace lcpp
{
    EZ_FORCE_INLINE
    MetaPropertyData::MetaPropertyData() :
        m_rawData(nullptr)
    {
    }

    template<typename T_Data>
    EZ_FORCE_INLINE
    lcpp::MetaPropertyData::MetaPropertyData(const T_Data& data) :
        m_rawData(nullptr)
    {
        EZ_CHECK_AT_COMPILETIME(sizeof(T_Data) <= sizeof(m_rawData));

        reinterpret_cast<T_Data&>(m_rawData) = data;
    }

    template<typename T_Data>
    EZ_FORCE_INLINE
    T_Data& lcpp::MetaPropertyData::as()
    {
        return reinterpret_cast<T_Data&>(m_rawData);
    }

    template<typename T_Data>
    EZ_FORCE_INLINE
    const T_Data& lcpp::MetaPropertyData::as() const
    {
        return reinterpret_cast<const T_Data&>(m_rawData);
    }

    EZ_FORCE_INLINE
    MetaProperty::MetaProperty() :
        m_id(),
        m_data()
    {
    }

    EZ_FORCE_INLINE
    MetaProperty::MetaProperty(MetaPropertyId id, MetaPropertyData data) :
        m_id(id),
        m_data(data)
    {
    }

    EZ_FORCE_INLINE
    MetaPropertyId MetaProperty::getId() const
    {
        return m_id;
    }

    EZ_FORCE_INLINE
    MetaPropertyData MetaProperty::getData() const
    {
        return m_data;
    }

    EZ_FORCE_INLINE
    MetaInfo::MetaInfo() :
        m_type(Type::ENUM_MIN),
        m_attributes(AttributeFlags::None),
        m_prettyName(m_type.toString())
    {
    }

    EZ_FORCE_INLINE
    MetaInfo::MetaInfo(const Type& type) :
        m_type(type),
        m_attributes(AttributeFlags::None),
        m_prettyName(m_type.toString())
    {
    }

    EZ_FORCE_INLINE
    MetaInfo::MetaInfo(const Type& type,
                       const String& prettyName) :
        m_type(type),
        m_attributes(AttributeFlags::None),
        m_prettyName(prettyName)
    {
    }

    EZ_FORCE_INLINE
    MetaInfo::MetaInfo(const Type& type,
                       const AttributeFlags& attributes,
                       const String& prettyName) :
        m_type(type),
        m_attributes(attributes),
        m_prettyName(prettyName)
    {
    }

    EZ_FORCE_INLINE
    const Type& MetaInfo::getType() const
    {
        return m_type;
    }

    EZ_FORCE_INLINE
    void MetaInfo::setType(const Type& type)
    {
        m_type = type;
    }

    EZ_FORCE_INLINE
    const AttributeFlags& MetaInfo::getAttributes() const
    {
        return m_attributes;
    }

    EZ_FORCE_INLINE
    void MetaInfo::setAttributes(const AttributeFlags& attributes)
    {
        m_attributes = attributes;
    }

    EZ_FORCE_INLINE
    const char* MetaInfo::getPrettyName() const
    {
        return m_prettyName.GetData();
    }

    EZ_FORCE_INLINE
    void MetaInfo::setPrettyName(const char* szPrettyName)
    {
        m_prettyName = szPrettyName;
    }

    EZ_FORCE_INLINE
    ezResult MetaInfo::getProperty(MetaPropertyId id, MetaProperty& out_prop) const
    {
        return m_properties.TryGetValue(id, out_prop) ? EZ_SUCCESS : EZ_FAILURE;
    }

    EZ_FORCE_INLINE
    void MetaInfo::addProperty(const MetaProperty& prop)
    {
        m_properties[prop.getId()] = prop;
    }
}
