#pragma once
#include "lcpp/core/schemeTypes/object.h"

namespace lcpp
{
    class SchemeCons :
        public SchemeObject
    {
    public:
        SCHEME_TYPE_DECLARATION(Cons);

        SchemeCons(const SchemeObject& car, const SchemeObject& cdr);
        virtual ~SchemeCons();

        virtual const SchemeBool& operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

        inline const SchemeObject& car() const { return *m_car; }
        inline void car(const SchemeObject& value){ m_car = &value; }
        inline const SchemeObject& cdr() const { return *m_cdr; }
        inline void cdr(const SchemeObject& value){ m_cdr = &value; }

    private:

        const SchemeObject* m_car;
        const SchemeObject* m_cdr;
    };

}
