#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

namespace lcpp
{
    class LCPP_CORE_API LispCons :
        public LispObject
    {
        friend class TypeFactory;
    public:

        static Ptr<LispCons> create(Ptr<LispObject> pCar, Ptr<LispObject> pCdr);
        static const Type& typeInfo();

    public:

        LispCons(const LispCons& toCopy);
        LispCons(LispCons&& toMove);

        virtual bool operator==(const LispObject& obj) const LCPP_OVERRIDE;
        bool operator==(const LispCons& rhs) const;

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual String toString() const LCPP_OVERRIDE;

            /// Prints the contents of itself to \a builder.
            /// \remark Effectlively omits the enclosing parenthesis.
        void toStringHelper(ezStringBuilder& builder) const;

            /// \brief set car.
        void car(Ptr<LispObject> pObject);

            /// \brief get car.
        Ptr<LispObject> car();

            /// \brief set cdr.
        void cdr(Ptr<LispObject> pObject);

            /// \brief get cdr
        Ptr<LispObject> cdr();
    private:

        Ptr<LispObject> m_pCar;
        Ptr<LispObject> m_pCdr;

        LispCons(Ptr<LispObject> pCar, Ptr<LispObject> pCdr);
    };

    /// Counts the elements of pRegularList
    ezResult count(Ptr<LispCons> pRegularList, ezUInt32& out_count);

    template<typename T_Array>
    void toArray(Ptr<LispObject> pObject, T_Array& out_array);
}

#include "lcpp/core/typeSystem/types/implementation/cons.inl"
