#pragma once

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"

namespace lcpp
{

    /// \brief This class is meant to have a unique, immutable instance for every needed type in the system.
    class MetaInfo
    {
    public:

        MetaInfo(const Type& type);

        MetaInfo(const Type& type, const String& prettyName);

        MetaInfo(const Type& type, const AttributeFlags& attributes, const String& prettyName);

        const Type& getType() const;
        const AttributeFlags& getAttributes() const;

        const String& getPrettyName() const;

    private:

        Type m_type;
        AttributeFlags m_attributes;

        String m_prettyName;

    };
}

#include "lcpp/core/typeSystem/impl/metaInfo.inl"
