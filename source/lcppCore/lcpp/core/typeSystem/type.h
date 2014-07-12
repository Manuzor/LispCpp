#pragma once

#include "lcpp/foundation/memoryInfo.h"

namespace lcpp
{
        /// \brief Describes a scheme type
    struct Type
    {
        inline static Type create(const char* typeName, const char* name, MemoryInfo memInfo)
        {
            return Type(ezHashing::MurmurHash(typeName), typeName, name, memInfo);
        }

            /// \brief This can be used to make static_asserts trigger at places in
            /// the code that need to be updated to the new version.
        enum { Version = 3 };

            /// \brief Is set automatically and will be unique for every new instance.
        const ezUInt32 id;

            /// \brief The name of type, e.g. LispNil
        const char* typeName;

            /// \brief A human-readable name of this type, e.g. Nil
        const char* name;

        MemoryInfo memory;

    private:
        Type(ezUInt32 id, const char* typeName, const char* name, MemoryInfo memInfo);

        // Disallow assignment
        void operator = (const Type&);
    };

    bool operator ==(const Type& lhs, const Type& rhs);
    bool operator !=(const Type& lhs, const Type& rhs);

        /// \brief Class that provides information about scheme types statically.
        /// \remark You have to specialize this template for all your scheme types!
        ///         In order to make this easier, you can use the macros defined at
        ///         the end of this file.
    template<typename T>
    struct TypeInfo
    {
            /// \brief Gets the instance of the Type associated with T
        static const Type& type();
    };
}

#define DECLARE_SCHEME_TYPE_INFO_WITH_NAME(theType, theName) template<> \
    struct TypeInfo<theType>                                            \
    {                                                                   \
        inline static const Type& type()                                \
        {                                                               \
            static_assert(Type::Version == 3,                           \
                "Type version was updated."                             \
                "Adjust your implementation accordingly!");             \
            static auto instance = Type::create(                        \
                #theType,                                               \
                theName,                                                \
                MemoryInfo(sizeof(theType), EZ_ALIGNMENT_OF(theType))   \
            );                                                          \
            return instance;                                            \
        }                                                               \
    }

#define DECLARE_SCHEME_TYPE_INFO(type) DECLARE_SCHEME_TYPE_INFO_WITH_NAME(type, #type)


#include "lcpp/core/typeSystem/implementation/type.inl"
