#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

namespace lcpp
{
    class SchemeCons :
        public SchemeExtend<SchemeCons, SchemeObject>
    {
        friend class TypeFactory;
    public:

        SchemeCons(const SchemeCons& toCopy);
        SchemeCons(SchemeCons&& toMove);

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeCons& rhs) const;

        virtual ezString toString() const LCPP_OVERRIDE;

        /// \brief set car.
        void car(Ptr<SchemeObject> pObject);
        /// \brief get car.
        Ptr<SchemeObject> car();

        /// \brief set cdr.
        void cdr(Ptr<SchemeObject> pObject);
        /// \brief get cdr
        Ptr<SchemeObject> cdr();
    private:

        Ptr<SchemeObject> m_pCar;
        Ptr<SchemeObject> m_pCdr;

        SchemeCons(Ptr<SchemeObject> pCar, Ptr<SchemeObject> pCdr);
        void toStringHelper(ezStringBuilder& builder) const;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeCons);
}

#include "lcpp/core/typeSystem/types/implementation/cons_inl.h"
