#pragma once

#include "lcpp/foundation/memoryInfo.h"

namespace lcpp
{
        /// \brief Describes a scheme type
    struct Type
    {
        inline static Type create(const ezUInt32 flags,
                                  const char* typeName,
                                  const char* name,
                                  const MemoryInfo& memInfo)
        {
            return Type(ezHashing::MurmurHash(typeName), flags, typeName, name, memInfo);
        }

        struct Flags
        {
            enum Enum
            {
                None = 0x0000,

                Abstract    = EZ_BIT(0),
                UserDefined = EZ_BIT(1), ///< If this flag is not set, it is built-in.
                Syntax      = EZ_BIT(2),
                Callable    = EZ_BIT(3),
                Arithmetic  = EZ_BIT(4),
                Singleton   = EZ_BIT(5),

                ALL = 0xFFFF,
            };
        };

            /// \brief Is set automatically and will be unique for every new instance.
        const ezUInt32 id;

            /// \brief A combination of \c Flags::Enum values.
        const ezUInt32 flags;

            /// \brief The name of type, e.g. LispNil
        const char* const typeName;

            /// \brief A human-readable name of this type, e.g. Nil
        const char* const name;

        const MemoryInfo memory;

    private:
        Type(ezUInt32 id,
             ezUInt32 flags,
             const char* typeName,
             const char* name,
             const MemoryInfo& memInfo);

        // Disallow assignment
        void operator = (const Type&);
    };

    bool operator ==(const Type& lhs, const Type& rhs);
    bool operator !=(const Type& lhs, const Type& rhs);

}

#include "lcpp/core/typeSystem/implementation/type.inl"
