#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/instancing.h"
#include "lcpp/core/typeSystem/types/numberDeclarations.h"

namespace lcpp
{
    class LCPP_CORE_API LispInteger :
        public LispObject
    {
        friend InstanceTable_Integers;
    public:

        typedef LispIntegerType Number_t;

        static Ptr<LispInteger> create(Number_t value);
        static const Type& typeInfo();

    private:

            /// \brief Creates a new instance without checking for an existing instance for the given \a value.
        static Ptr<LispInteger> createNew(Number_t value);

    public:

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;
        virtual const Type& type() const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;
        virtual bool operator ==(const LispObject& obj) const LCPP_OVERRIDE;

        Number_t value() const;
        void     value(Number_t val);

    private:

        Number_t m_value;

        explicit LispInteger(Number_t value);
        LispInteger(const LispInteger& toCopy);
    };

    bool operator ==(const LispInteger& lhs, const LispInteger& rhs);

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

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;
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

    bool operator ==(const LispNumber& lhs, const LispNumber& rhs);
    bool operator ==(const LispInteger& lhs, const LispNumber& rhs);
    bool operator ==(const LispNumber& lhs, const LispInteger& rhs);
}

#include "lcpp/core/typeSystem/types/implementation/number.inl"
