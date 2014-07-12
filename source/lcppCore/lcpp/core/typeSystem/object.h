#pragma once
#include "lcpp/core/typeSystem/type.h"

namespace lcpp
{
    class TypeFactory;

    template<typename Derived, typename Base>
    class LispExtend;

    /// \brief Base class for all scheme types
    /// 
    /// Do not directly derive your scheme type from this.
    /// Use LispExtend instead.
    /// Example:
    /// class LispBool : public LispExtend<LispBool, LispObject> { /* ... */ };
    class LCPP_CORE_API LispObject
    {
        friend class TypeFactory;
    public:

        virtual ~LispObject() {}

        virtual bool operator ==(const LispObject& other) const = 0;

        bool operator !=(const LispObject& rhs) const;

        virtual ezString toString() const = 0;

        virtual const Type& type() const = 0;

        virtual Ptr<LispObject> clone(ezAllocatorBase* pAllocator) const = 0;

        template<typename T_Other>
        bool is() const;

    private:
        LCPP_DISALLOW_COPY_ASSIGNMENT(LispObject);
    };

    template<>
    struct TypeInfo< LispObject >
    {
        inline static const Type& type()
        {
            static_assert(Type::Version == 2,
                "Type version was updated. Adjust your implementation accordingly!");
            static auto theType = Type::create(
                "LispObject",
                MemoryInfo(sizeof(LispObject), sizeof(LispObject))
                );
            return theType;
        }
    };

    /// \brief Class used to generate default overrides of LispObject.
    ///
    /// Has to derive from LispObject at some point!
    template<typename T_Derived, typename T_Base>
    class LispExtend : public T_Base
    {
    public:
        virtual ~LispExtend() = 0 {}

        virtual const Type& type() const LCPP_OVERRIDE;
        virtual Ptr<LispObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;

    private:
        LCPP_DISALLOW_COPY_ASSIGNMENT(LispExtend);
    };
}

#include "lcpp/core/typeSystem/implementation/object_inl.h"
