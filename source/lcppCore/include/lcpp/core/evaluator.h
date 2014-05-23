#pragma once
#include "lcpp/core/typeSystem/typeFactory.h"
#include "lcpp/core/environment.h"

namespace lcpp
{
    namespace exceptions {

        class InvalidEvalInput :
            public ExceptionBase
        {
        public:
            inline InvalidEvalInput(const char* message = nullptr, const char* file = nullptr, ezUInt32 line = -1) :
                ExceptionBase(message ? message : "Invalid operation!", file, line)
            {
            }
        };
    }

    //////////////////////////////////////////////////////////////////////////

    class SchemeObject;

    class LCPP_CORE_API IEvaluator
    {
    public:
        virtual ~IEvaluator() {}

        virtual SchemeObject& evalulate(SchemeObject& object) = 0;

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

        virtual SchemeObject& evalulate(SchemeObject& object) override;

        virtual Environment& environment() override;
        virtual const Environment& environment() const override;

    private:
        TypeFactory m_defaultFactory;
        TypeFactory* m_pFactory;

        Environment m_env;
    };
    
}
