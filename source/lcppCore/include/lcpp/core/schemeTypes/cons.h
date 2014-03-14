#pragma once
#include "lcpp/core/schemeTypes/object.h"

#include "lcpp/foundation/common.h"

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
        
        /// \brief get cdr
        const SchemeObject& cdr() const;

    private:

        static ezAllocatorBase& getAllocator();

        void toStringHelper(ezStringBuilder& builder) const;

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
