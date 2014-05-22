#pragma once
#include "lcpp/core/typeSystem/typeFactory.h"

namespace lcpp
{
    class SchemeObject;

    class LCPP_CORE_API IEvaluator
    {
    public:
        virtual ~IEvaluator() {}

        virtual SchemeObject& evalulate(SchemeObject& object) = 0;
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

    private:
        TypeFactory m_defaultFactory;
        TypeFactory* m_pFactory;
    };
    
}
