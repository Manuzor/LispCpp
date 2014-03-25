#pragma once

namespace lcpp
{
    /// \brief Describes a scheme type
    struct SchemeTypeId
    {
        /// Is set automatically and will be unique for every new instance.
        const ezUInt64 id;
        
        /// \brief A human-readable name of this type.
        const char* name;

        /// \brief Number of bytes of this scheme type. Typically sizeof(T).
        size_t size;

        size_t alignment;


        SchemeTypeId();

    private:
        static ezUInt64 makeUniqueId();

        // Disallow assignment
        void operator =(const SchemeTypeId&);
    };

    bool operator ==(const SchemeTypeId& lhs, const SchemeTypeId& rhs);

    /// \brief Class that provides information about scheme types statically.
    /// \remark You have to specialize this template for all your scheme types!
    ///         In order to make this easier, you can use the macros defined at
    ///         the end of this file.
    template<typename T>
    struct SchemeTypeInfo
    {
        /// \brief Gets the instance of the SchemeType associated with T
        static const SchemeTypeId& type();
    };
}

#define DECLARE_SCHEME_TYPE_INFO_WITH_NAME(theType, theName) template<> \
    struct SchemeTypeInfo<theType>                                      \
    {                                                                   \
        static const SchemeTypeId& type()                               \
        {                                                               \
            static SchemeTypeId instance;                               \
            instance.name = theName;                                    \
            instance.size = sizeof(theType);                            \
            instance.alignment = EZ_ALIGNMENT_OF(theType);              \
            return instance;                                            \
        }                                                               \
    }

#define DECLARE_SCHEME_TYPE_INFO(type) DECLARE_SCHEME_TYPE_INFO_WITH_NAME(type, #type)


#include "lcpp/core/typeSystem/implementation/type_inl.h"
