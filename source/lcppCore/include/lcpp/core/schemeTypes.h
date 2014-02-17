#pragma once

namespace lcpp
{
    class SchemeObject;
    class SchemeBool;
    class SchemeNil;

    //////////////////////////////////////////////////////////////////////////

    const SchemeBool& convert(bool value);

    //////////////////////////////////////////////////////////////////////////

    struct SchemeType
    {
        enum Enum
        {
            Object,
            Void,
            Nil,
            Bool,
            Cons,
            Number,
            String
        };

        LCPP_DISALLOW_CONSTRUCTION(SchemeType);
    };

#define SCHEME_TYPE_DECLARATION(typeValue) virtual ::lcpp::SchemeType::Enum type() const override { return ::lcpp::SchemeType::typeValue; }\
    virtual const ::lcpp::SchemeBool& is(::lcpp::SchemeType::Enum type) const override { return convert(type == ::lcpp::SchemeType::typeValue); }

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeObject
    {
    public:

        virtual ~SchemeObject() = 0 {}
        virtual const SchemeBool& operator==(const SchemeObject& other) const = 0;
        virtual ezString toString() const = 0;

        virtual SchemeType::Enum type() const { return SchemeType::Object; }
        virtual const SchemeBool& is(SchemeType::Enum type) const { return convert(type == SchemeType::Object); }
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeVoid :
        public SchemeObject
    {
    public:
        SCHEME_TYPE_DECLARATION(Void);

        SchemeVoid();
        virtual ~SchemeVoid();

        virtual const SchemeBool& operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

    private:
        EZ_DISALLOW_COPY_AND_ASSIGN(SchemeVoid);
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeBool :
        public SchemeObject
    {
    public:
        SCHEME_TYPE_DECLARATION(Bool);

        SchemeBool();
        virtual ~SchemeBool();

        virtual const SchemeBool& operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

        operator bool() const;

    private:
        EZ_DISALLOW_COPY_AND_ASSIGN(SchemeBool);
    };

    //////////////////////////////////////////////////////////////////////////

    template<typename NUMBER_TYPE>
    class SchemeNumber :
        public SchemeObject
    {
    public:
        SCHEME_TYPE_DECLARATION(Number);

        inline SchemeNumber(NUMBER_TYPE value);
        inline virtual ~SchemeNumber();

        inline NUMBER_TYPE value() const { return m_value; }
        inline void value(NUMBER_TYPE value) const { m_value = value; }

    private:

        NUMBER_TYPE m_value;
    };

    typedef SchemeNumber<ezInt32> SchemeInt;
    typedef SchemeNumber<ezUInt32> SchemeUInt;
    typedef SchemeNumber<float> SchemeFloat;

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeCons :
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

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeNil :
        public SchemeObject
    {
    public:
        SCHEME_TYPE_DECLARATION(Nil);

        SchemeNil();
        virtual ~SchemeNil();

        virtual const SchemeBool& operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

    private:
        EZ_DISALLOW_COPY_AND_ASSIGN(SchemeNil);
    };

    //////////////////////////////////////////////////////////////////////////

    // namespace for all singletons such as bool types and void.
    namespace singletons
    {
        extern LCPP_CORE_API const SchemeVoid g_void;
        extern LCPP_CORE_API const SchemeNil g_nil;
        extern LCPP_CORE_API const SchemeBool g_true;
        extern LCPP_CORE_API const SchemeBool g_false;
    };

    // Easy access macros for all singleton instances
    #define SCHEME_VOID  ::lcpp::singletons::g_void
    #define SCHEME_NIL   ::lcpp::singletons::g_nil
    #define SCHEME_TRUE  ::lcpp::singletons::g_true
    #define SCHEME_FALSE ::lcpp::singletons::g_false

#include "lcpp/core/implementation/schemeTypes_inl.h"
}