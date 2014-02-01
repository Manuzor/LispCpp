#pragma once

namespace lcpp
{
    class SchemeBool;

    class SchemeObject
    {
    public:
        virtual ~SchemeObject() = 0 {}
        virtual const SchemeBool& operator==(const SchemeObject& other) const = 0;
        virtual String toString() const = 0;
    };


#pragma region Void
    class LCPP_CORE_API SchemeVoid :
        public SchemeObject
    {
    public:
        inline SchemeVoid(){}
        inline virtual ~SchemeVoid(){}

        virtual const SchemeBool& operator==(const SchemeObject& obj) const;
        inline virtual String toString() const override
        {
            return _T("#v");
        }

    private:
        inline SchemeVoid(const SchemeVoid&){}
        inline void operator=(const SchemeVoid&){}
    };
#pragma endregion

#pragma region Nil
    class LCPP_CORE_API SchemeNil :
        public SchemeObject
    {
    public:
        inline SchemeNil(){}
        inline virtual ~SchemeNil(){}

        virtual const SchemeBool& operator==(const SchemeObject& obj) const;
        virtual String toString() const override;

    private:
        inline SchemeNil(const SchemeNil&){}
        inline void operator=(const SchemeNil&){}
    };
    const SchemeNil g_nil;
#pragma endregion

#pragma region Bool
    class LCPP_CORE_API SchemeBool :
        public SchemeObject
    {
    public:
        inline SchemeBool(){}
        inline virtual ~SchemeBool(){}

        static const SchemeBool& create(bool value);

        virtual const SchemeBool& operator==(const SchemeObject& obj) const;
        virtual String toString() const override;

        operator bool() const;

    private:
        inline SchemeBool(const SchemeBool&){}
        inline void operator=(const SchemeBool&){}
    };
    const SchemeBool g_true;
    const SchemeBool g_false;
#pragma endregion

    class SchemeNumber :
        public SchemeObject
    {
    public:
    	inline SchemeNumber(){}
    	inline virtual ~SchemeNumber(){}
    };

#pragma region Cons
    class LCPP_CORE_API SchemeCons :
        public SchemeObject
    {
    public:
        inline SchemeCons(const SchemeObject& car, const SchemeObject& cdr = g_nil) :
            m_car(&car),
            m_cdr(&cdr)
        {}
        inline virtual ~SchemeCons(){}

        virtual const SchemeBool& operator==(const SchemeObject& obj) const override;
        inline virtual String toString() const override
        {
            StringStream stream;
            stream << _T("(") << m_car->toString() << _T(" . ") << m_cdr->toString() << _T(")");
            return stream.str();
        }

        const SchemeObject& car() const { return *m_car; }
        void car(const SchemeObject& value){ m_car = &value; }
        const SchemeObject& cdr() const { return *m_cdr; }
        void cdr(const SchemeObject& value){ m_cdr = &value; }

    private:

        const SchemeObject* m_car;
        const SchemeObject* m_cdr;
    };
#pragma endregion
    
    String toString(const char* str)
    {
        return _T("");
    }

}