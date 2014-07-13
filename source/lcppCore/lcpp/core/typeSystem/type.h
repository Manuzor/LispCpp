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

}

#include "lcpp/core/typeSystem/implementation/type.inl"
