#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

namespace lcpp
{
    class SchemeCons :
        public SchemeExtend<SchemeCons, SchemeObject>
    {
    public:

        SchemeCons(SchemeObject& car, SchemeObject& cdr);

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

        void toStringHelper(ezStringBuilder& builder) const;

        SchemeObject* m_pCar;
        SchemeObject* m_pCdr;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeCons);
}

#include "lcpp/core/typeSystem/types/implementation/cons_inl.h"
