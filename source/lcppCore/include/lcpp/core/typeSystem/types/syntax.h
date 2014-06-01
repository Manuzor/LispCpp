#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class IEvaluator;
    class Environment;
    class SchemeSymbol;
    class SchemeCons;

    class LCPP_CORE_API SchemeSyntax :
        public SchemeExtend<SchemeSyntax, SchemeObject>
    {
    public:
        typedef Ptr<SchemeObject>(*HandlerFuncPtr_t)(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgList);

        SchemeSyntax(Ptr<SchemeSymbol> pName,
                     Ptr<SchemeCons> pUnevaluatedArgList,
                     HandlerFuncPtr_t pHandler);

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeSyntax& rhs) const;

        virtual ezString toString() const LCPP_OVERRIDE;

        virtual Ptr<SchemeObject> call(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator);

        Ptr<SchemeSymbol> name();
        Ptr<const SchemeSymbol> name() const;

    protected:
        Ptr<SchemeSymbol> m_pName;
        Ptr<SchemeCons> m_pUnevaluatedArgList;
        HandlerFuncPtr_t m_pHandler;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeSyntax);
}

#include "lcpp/core/typeSystem/types/implementation/syntax_inl.h"
