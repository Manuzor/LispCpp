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

        virtual ~SchemeCons();

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeCons& rhs) const;

        virtual ezString toString() const LCPP_OVERRIDE;

        /// \brief set car.
        void car(SchemeObject& object);
        /// \brief get car.
        SchemeObject& car();

        /// \brief set cdr.
        void cdr(SchemeObject& object);
        /// \brief get cdr
        SchemeObject& cdr();
    private:

        SchemeObject* m_pCar;
        SchemeObject* m_pCdr;

        SchemeCons(SchemeObject& car, SchemeObject& cdr);
        void toStringHelper(ezStringBuilder& builder) const;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeCons);
}

#include "lcpp/core/typeSystem/types/implementation/cons_inl.h"
