#pragma once

namespace lcpp
{
    class SchemeObject;
    class SchemeBool;
    class SchemeNil;

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

        template<typename T>
        inline static SchemeType::Enum of(const T& instance)
        {
            return T::Type;
        }

        template<>
        inline static SchemeType::Enum of<SchemeObject>(const SchemeObject& instance)
        {
            return SchemeType::Object;
        }

    private:
        DISALLOW_COPY_AND_ASSIGNMENT(SchemeType);
        SchemeType();
    };

    #define SCHEME_TYPE_DECLARATION(type) static const SchemeType::Enum Type = SchemeType::type;

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeObject
    {
    public:

        virtual ~SchemeObject() = 0 {}
        virtual const SchemeBool& operator==(const SchemeObject& other) const = 0;
        virtual ezString toString() const = 0;

        virtual const SchemeBool& is(SchemeType::Enum type) const;
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
        DISALLOW_COPY_AND_ASSIGNMENT(SchemeVoid);
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeBool :
        public SchemeObject
    {
    public:
        SCHEME_TYPE_DECLARATION(Bool);

        SchemeBool();
        virtual ~SchemeBool();

        static const SchemeBool& create(bool value);

        virtual const SchemeBool& operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

        operator bool() const;

    private:
        DISALLOW_COPY_AND_ASSIGNMENT(SchemeBool);
    };

    //////////////////////////////////////////////////////////////////////////

    template<typename NUMBER_TYPE>
    class SchemeNumber :
        public SchemeObject
    {
    public:
        SCHEME_TYPE_DECLARATION(Number);

        SchemeNumber(NUMBER_TYPE value);
        virtual ~SchemeNumber();

        inline NUMBER_TYPE value() const { return m_value; }
        inline void value(NUMBER_TYPE value) const { m_value = value; }

    private:

        NUMBER_TYPE m_value;
    };

    //////////////////////////////////////////////////////////////////////////

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
        DISALLOW_COPY_AND_ASSIGNMENT(SchemeNil);
    };

    //////////////////////////////////////////////////////////////////////////

    // namespace for all singletons such as bool types and void.
    namespace singletons
    {
        extern ezStatic<const SchemeVoid> g_void;
        extern ezStatic<const SchemeBool> g_true;
        extern ezStatic<const SchemeBool> g_false;
        extern ezStatic<const SchemeNil> g_nil;
    };

    // Easy access macros for all singleton instances
    #define SCHEME_VOID  ::lcpp::singletons::g_void.GetStatic()
    #define SCHEME_NIL   ::lcpp::singletons::g_nil.GetStatic()
    #define SCHEME_TRUE  ::lcpp::singletons::g_true.GetStatic()
    #define SCHEME_FALSE ::lcpp::singletons::g_false.GetStatic()

#include "implementation/schemeTypes_inl.h"
}