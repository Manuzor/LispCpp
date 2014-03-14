#pragma once
#include "lcpp/core/schemeTypes/object.h"

namespace lcpp
{
    class SchemeCons :
        public SchemeExtend<SchemeCons, SchemeObject>
    {
    public:

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
        /// \brief set car by copying \a value
        void car(const SchemeObject& value);
        
        /// \brief get cdr
        const SchemeObject& cdr() const;
        /// \brief set cdr by copying \a value
        void cdr(const SchemeObject& value);

    private:

        const SchemeObject* m_car;
        const SchemeObject* m_cdr;

        void set(const SchemeObject*& member, const SchemeObject& from);
    };

    template<>
    struct SchemeTypeInfo<SchemeCons>
    {
        inline static size_t size() { return sizeof(SchemeCons); }
        inline static SchemeType::Enum type() { return SchemeType::Cons; }
        inline static const char* name() { return "SchemeCons"; }
    };
}
