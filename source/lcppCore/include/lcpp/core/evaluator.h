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

        virtual Ptr<SchemeObject> evalulate(Ptr<SchemeObject> pObject) = 0;
        virtual Ptr<SchemeObject> evalulate(Ptr<Environment> pEnv, Ptr<SchemeObject> pObject) = 0;

        virtual Environment& environment() = 0;
        virtual const Environment& environment() const = 0;
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

        virtual Ptr<SchemeObject> evalulate(Ptr<SchemeObject> pObject) override;
        virtual Ptr<SchemeObject> evalulate(Ptr<Environment> pEnv, Ptr<SchemeObject> pObject) override;

        virtual Environment& environment() override;
        virtual const Environment& environment() const override;

    private:
        TypeFactory m_defaultFactory;
        TypeFactory* m_pFactory;

        Environment m_syntax;
        Environment m_env;

        void setupSyntax();
        void setupEnvironment();
    };
    
}
