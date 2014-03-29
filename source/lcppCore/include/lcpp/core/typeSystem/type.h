#pragma once

#include "lcpp/foundation/memoryInfo.h"

namespace lcpp
{
    /// \brief Describes a scheme type
    struct Type
    {
        /// \brief This can be used to make static_asserts trigger at places in
        /// the code that need to be updated to the new version.
        enum{ Version = 2 };

        /// \brief Is set automatically and will be unique for every new instance.
        const ezUInt64 id;
        
        /// \brief A human-readable name of this type.
        const char* name;

        MemoryInfo memory;

        Type();

    private:
        static ezUInt64 makeUniqueId();

        // Disallow assignment
        void operator =(const Type&);
    };

    bool operator ==(const Type& lhs, const Type& rhs);

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
            static_assert(Type::Version == 2,                           \
                "Type version was updated."                             \
                "Adjust your implementation accordingly!");             \
            static Type instance;                                       \
            instance.name = theName;                                    \
            instance.memory.size = sizeof(theType);                     \
            instance.memory.alignment = EZ_ALIGNMENT_OF(theType);       \
            return instance;                                            \
        }                                                               \
    }

#define DECLARE_SCHEME_TYPE_INFO(type) DECLARE_SCHEME_TYPE_INFO_WITH_NAME(type, #type)


#include "lcpp/core/typeSystem/implementation/type_inl.h"
