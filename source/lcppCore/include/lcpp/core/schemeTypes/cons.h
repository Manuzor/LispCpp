#pragma once
#include "lcpp/core/schemeTypes/object.h"

namespace lcpp
{
    class SchemeCons :
        public SchemeObject
    {
    public:
        LCPP_SCHEME_TYPE_DECLARATION(Cons);

        SchemeCons();
        explicit SchemeCons(const SchemeObject& car);
        SchemeCons(const SchemeObject& car, const SchemeObject& cdr);

        SchemeCons(const SchemeCons& copy);
        void operator =(SchemeCons copyAssign);

        SchemeCons(SchemeCons&& toMove);

        virtual ~SchemeCons();

        virtual bool operator==(const SchemeObject& obj) const override;
        bool operator==(const SchemeCons& rhs) const;

        virtual ezString toString() const override;

        /// \brief get car
        const SchemeObject& car() const;
        /// \brief set car
        void car(const SchemeObject& value);
        
        /// \brief get cdr
        const SchemeObject& cdr() const;
        /// \brief set cdr
        void cdr(const SchemeObject& value);

    private:

        const SchemeObject* m_car;
        const SchemeObject* m_cdr;
    };

}
