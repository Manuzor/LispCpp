#pragma once

namespace lcpp
{
    class SchemeNil;
    class SchemeBool;
    class SchemeCons;

    class SchemeObject
    {
    public:
        virtual ~SchemeObject() = 0 {}
    };

    class SchemeNil :
        public SchemeObject
    {
    public:
        inline virtual ~SchemeNil(){}

        const SchemeBool& operator==(const SchemeObject& obj) const;
    } g_nil;

    class SchemeBool
    {
    public:
        inline SchemeBool(){}
        inline virtual ~SchemeBool(){}

        static const SchemeBool& create(bool value);
    };


    class SchemeCons :
        public SchemeObject
    {
    public:
        inline SchemeCons(const SchemeObject& car, const SchemeObject& cdr = g_nil) :
            m_car(car),
            m_cdr(cdr)
        {}
        inline virtual ~SchemeCons(){  }

    private:

        const SchemeObject& m_car;
        const SchemeObject& m_cdr;
    };

    namespace constants
    {
        const SchemeNil Nil;
        const SchemeBool True;
        const SchemeBool False;
    }
}