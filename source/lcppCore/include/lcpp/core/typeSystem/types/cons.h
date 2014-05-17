#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

namespace lcpp
{
    class SchemeCons :
        public SchemeExtend<SchemeCons, SchemeObject>
    {
    public:

        explicit SchemeCons(SchemeObject& car = SCHEME_NIL, SchemeObject& cdr = SCHEME_NIL);

        virtual ~SchemeCons();

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeCons& rhs) const;

        virtual ezString toString() const LCPP_OVERRIDE;

        /// \brief get car
        SchemeObject& car() const;
        
        /// \brief get cdr
        SchemeObject& cdr() const;

    private:

        void toStringHelper(ezStringBuilder& builder) const;

        SchemeObject* m_pCar;
        SchemeObject* m_pCdr;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeCons);
}

#include "lcpp/core/typeSystem/types/implementation/cons_inl.h"
