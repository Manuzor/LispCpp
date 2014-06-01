#pragma once
#include "lcpp/core/typeSystem/typeFactory.h"
#include "lcpp/core/environment.h"

namespace lcpp
{
    class SchemeObject;

    class LCPP_CORE_API IEvaluator
    {
    public:
        virtual ~IEvaluator() {}

        virtual void initialize() = 0;

        virtual Ptr<SchemeObject> evalulate(Ptr<SchemeObject> pObject) = 0;
        virtual Ptr<SchemeObject> evalulate(Ptr<Environment> pEnv, Ptr<SchemeObject> pObject) = 0;

        virtual Environment& environment() = 0;
        virtual const Environment& environment() const = 0;

        virtual Ptr<TypeFactory> factory() = 0;
        virtual Ptr<const TypeFactory> factory() const = 0;
    };

    class LCPP_CORE_API RecursiveEvaluator : public IEvaluator
    {
    public:
        struct CInfo
        {
            TypeFactory* pFactory;

            CInfo() :
                pFactory(nullptr)
            {
            }
        };
    public:
        explicit RecursiveEvaluator();
        explicit RecursiveEvaluator(const CInfo& cinfo);
        virtual ~RecursiveEvaluator();

        virtual void initialize() LCPP_OVERRIDE;

        virtual Ptr<SchemeObject> evalulate(Ptr<SchemeObject> pObject) LCPP_OVERRIDE;
        virtual Ptr<SchemeObject> evalulate(Ptr<Environment> pEnv, Ptr<SchemeObject> pObject) LCPP_OVERRIDE;

        virtual Environment& environment() LCPP_OVERRIDE;
        virtual const Environment& environment() const LCPP_OVERRIDE;

        virtual Ptr<TypeFactory> factory() LCPP_OVERRIDE;
        virtual Ptr<const TypeFactory> factory() const LCPP_OVERRIDE;

    private:
        TypeFactory m_defaultFactory;
        TypeFactory* m_pFactory;

        Environment m_syntax;
        Environment m_env;

        void evaluateEach(Ptr<Environment> pEnv, Ptr<SchemeCons> pCons);

        void setupSyntax();
        void setupEnvironment();
    };
    
}
