﻿#pragma once

#include "lcpp/foundation/memoryInfo.h"

namespace lcpp
{
        /// \brief Describes a lisp type
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
                UserDefined = EZ_BIT(1), ///< If this flag is not set, this type describes a built-in type.
                Syntax      = EZ_BIT(2),
                Callable    = EZ_BIT(3),
                Arithmetic  = EZ_BIT(4),
                Singleton   = EZ_BIT(5),
                Nameable    = EZ_BIT(6),

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

        // Convenience functions so the user does not have to use bitwise operators

        bool isAbstract() const;
        bool isUserDefined() const;
        bool isSyntax() const;
        bool isCallable() const;
        bool isArithmetic() const;
        bool isSingleton() const;
        bool isNameable() const;

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
