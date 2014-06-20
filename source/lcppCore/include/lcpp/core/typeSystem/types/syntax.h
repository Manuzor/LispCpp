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
        SchemeSyntax(Ptr<SchemeSymbol> pName,
                     Ptr<SchemeCons> pUnevaluatedArgList);

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeSyntax& rhs) const;

        virtual ezString toString() const LCPP_OVERRIDE;

        lcpp::Ptr<      lcpp::SchemeSymbol> lcpp::SchemeSyntax::name();
        lcpp::Ptr<const lcpp::SchemeSymbol> lcpp::SchemeSyntax::name() const;

        virtual Ptr<SchemeObject> call(Ptr<SchemeRuntime> pRuntime,
                                       Ptr<Environment> pEnv) = 0;

    protected:
        Ptr<SchemeSymbol> m_pName;
        Ptr<SchemeCons> m_pUnevaluatedArgList;
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
                             Ptr<SchemeCons> pUnevaluatedArgList,
                             HandlerFuncPtr_t pHandler);

        virtual Ptr<SchemeObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;

        virtual Ptr<SchemeObject> call(Ptr<SchemeRuntime> pRuntime,
                                       Ptr<Environment> pEnv) LCPP_OVERRIDE;

    private:
        HandlerFuncPtr_t m_pHandler;
    };
}

#include "lcpp/core/typeSystem/types/implementation/syntax_inl.h"
