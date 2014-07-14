#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/instancing.h"
#include "lcpp/core/typeSystem/types/numberDeclarations.h"

namespace lcpp
{
    class LCPP_CORE_API LispInteger :
        public LispObject
    {
    public:

        typedef LispIntegerType Number_t;

        static Ptr<LispInteger> create(Number_t value);
        static const Type& typeInfo();

    public:

        virtual Ptr<LispObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;
        virtual const Type& type() const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;
        virtual bool operator ==(const LispObject& obj) const LCPP_OVERRIDE;

        Number_t value() const;
        void     value(Number_t val);

    private:
        friend InstanceTable_Integers;

        /// \brief Creates a new instance without checking for an existing instance for the given \a value.
        static Ptr<LispInteger> createNew(Number_t value);

    private:

        Number_t m_value;

        explicit LispInteger(Number_t value);
        LispInteger(const LispInteger& toCopy);
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API LispNumber :
        public LispObject
    {
        friend class TypeFactory;
    public:

        typedef double Number_t;

        static Ptr<LispNumber> create(Number_t value);
        static const Type& typeInfo();

    public:

        virtual Ptr<LispObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;
        virtual const Type& type() const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;
        virtual bool operator ==(const LispObject& obj) const LCPP_OVERRIDE;

        Number_t value() const;
        void     value(Number_t val);

    private:
        Number_t m_value;

        explicit LispNumber(Number_t value);
        LispNumber(const LispNumber& toCopy);
    };
}

#include "lcpp/core/typeSystem/types/implementation/number_inl.h"
