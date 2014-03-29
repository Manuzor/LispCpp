#pragma once
#include "lcpp/core/typeSystem/type.h"

namespace lcpp
{
    template<typename Derived, typename Base>
    class SchemeExtend;

    /// \brief Base class for all scheme types
    /// 
    /// Do not directly derive your scheme type from this.
    /// Use SchemeExtend instead.
    /// Example:
    /// class SchemeBool : public SchemeExtend<SchemeBool, SchemeObject> { /* ... */ };
    class SchemeObject
    {
    public:

        virtual ~SchemeObject() = 0 {}
        virtual bool operator ==(const SchemeObject& other) const = 0;
        virtual bool operator !=(const SchemeObject& rhs) const;
        virtual ezString toString() const = 0;

        virtual const Type& type() const = 0;
        virtual bool is(const Type& type) const = 0;

        virtual void copyTo(void* mem) const = 0;
    };
    
    template<>
    struct TypeInfo< SchemeObject >
    {
        static const Type& type()
        {
            static_assert(Type::Version == 2,
                "Type version was updated. Adjust your implementation accordingly!");
            static Type theType;
            theType.name = "SchemeObject";
            theType.memory.size = theType.memory.alignment = sizeof(SchemeObject);
            return theType;
        }
    };

    /// \brief Class used to generate default overrides of SchemeObject.
    ///
    /// Has to derive from SchemeObject at some point!
    template<typename Derived, typename Base>
    class SchemeExtend : public Base
    {
    public:
        virtual ~SchemeExtend() = 0 {}

        virtual const Type& type() const override;
        virtual bool is(const Type& type) const override;

        virtual void copyTo(void* mem) const override;
    };
}

#include "lcpp/core/typeSystem/implementation/object_inl.h"
