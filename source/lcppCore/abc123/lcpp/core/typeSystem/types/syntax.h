#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeRuntime;
    class Environment;
    class SchemeSymbol;
    class SchemeCons;

    class LCPP_CORE_API SchemeSyntax :
        public SchemeObject
    {
    public:
        SchemeSyntax(Ptr<SchemeSymbol> pName);

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeSyntax& rhs) const;

        virtual ezString toString() const LCPP_OVERRIDE;

        void lcpp::SchemeSyntax::name(lcpp::Ptr<lcpp::SchemeSymbol> pName);
        lcpp::Ptr<      lcpp::SchemeSymbol> lcpp::SchemeSyntax::name();
        lcpp::Ptr<const lcpp::SchemeSymbol> lcpp::SchemeSyntax::name() const;

        virtual Ptr<SchemeObject> call(Ptr<SchemeRuntime> pRuntime,
                                       Ptr<Environment> pEnv,
                                       Ptr<SchemeObject> pUnevaluatedArgList) = 0;

    protected:
        Ptr<SchemeSymbol> m_pName;
    };

    template<>
    struct TypeInfo< SchemeSyntax >
    {
        inline static const Type& type()
        {
            static_assert(Type::Version == 2,
                          "Type version was updated. Adjust your implementation accordingly!");
            static auto theType = Type::create(
                "SchemeSyntax",
                MemoryInfo(sizeof(SchemeSyntax), sizeof(SchemeSyntax))
                );
            return theType;
        }
    };

    class SchemeSyntax_Builtin :
        public SchemeSyntax
    {
    public:
        typedef Ptr<SchemeObject>(*HandlerFuncPtr_t)(Ptr<SchemeRuntime>, Ptr<Environment>, Ptr<SchemeObject>);

        SchemeSyntax_Builtin(Ptr<SchemeSymbol> pName,
                             HandlerFuncPtr_t pHandler);

        virtual Ptr<SchemeObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;

        virtual Ptr<SchemeObject> call(Ptr<SchemeRuntime> pRuntime,
                                       Ptr<Environment> pEnv,
                                       Ptr<SchemeObject> pUnevaluatedArgList) LCPP_OVERRIDE;

    private:
        HandlerFuncPtr_t m_pHandler;
    };
}

#include "lcpp/core/typeSystem/types/implementation/syntax_inl.h"
