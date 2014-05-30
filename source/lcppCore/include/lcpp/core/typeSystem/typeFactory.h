#pragma once
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/function.h"

namespace lcpp
{
    class SchemeObject;
    class SchemeCons;
    class SchemeString;
    class SchemeSymbol;

    class Environment;
    class IEvaluator;

    class LCPP_CORE_API TypeFactory
    {
    public:
        TypeFactory(ezAllocatorBase* pAllocator = defaultAllocator());
        ~TypeFactory();

        Ptr<SchemeInteger>  createInteger(SchemeInteger::Number_t value);
        Ptr<SchemeNumber>   createNumber(SchemeNumber::Number_t value);
        Ptr<SchemeString>   createString(const ezString& str);
        Ptr<SchemeSymbol>   createSymbol(const ezString& symbol);
        Ptr<SchemeCons>     createCons(Ptr<SchemeObject> car, Ptr<SchemeObject> cdr);

        Ptr<SchemeFunction> createFunction(Ptr<Environment> pParentEnv,
                                           Ptr<IEvaluator> pEvaluator,
                                           Ptr<SchemeCons> pBody);
        Ptr<SchemeFunction> createFunction(Ptr<Environment> pParentEnv,
                                           Ptr<IEvaluator> pEvaluator,
                                           SchemeFunction::Executor executor);

    private:
        ezAllocatorBase* m_pAllocator;

        ezHashTable<ezString, Ptr<SchemeSymbol>> m_symbols;
        ezHashTable<SchemeInteger::Number_t, Ptr<SchemeInteger>> m_integers;
    };
}
