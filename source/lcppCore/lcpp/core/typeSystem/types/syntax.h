#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispRuntime;
    class Environment;
    class LispSymbol;
    class LispCons;

    class LCPP_CORE_API LispSyntax :
        public LispObject
    {
    public:
        LispSyntax(Ptr<LispSymbol> pName);

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual bool operator==(const LispObject& obj) const LCPP_OVERRIDE;
        bool operator==(const LispSyntax& rhs) const;

        virtual ezString toString() const LCPP_OVERRIDE;

        void lcpp::LispSyntax::name(lcpp::Ptr<lcpp::LispSymbol> pName);
        lcpp::Ptr<      lcpp::LispSymbol> lcpp::LispSyntax::name();
        lcpp::Ptr<const lcpp::LispSymbol> lcpp::LispSyntax::name() const;

        virtual Ptr<LispObject> call(Ptr<LispRuntime> pRuntime,
                                       Ptr<Environment> pEnv,
                                       Ptr<LispObject> pUnevaluatedArgList) = 0;

    protected:
        Ptr<LispSymbol> m_pName;
    };

    template<>
    struct TypeInfo< LispSyntax >
    {
        inline static const Type& type()
        {
            static_assert(Type::Version == 2,
                          "Type version was updated. Adjust your implementation accordingly!");
            static auto theType = Type::create(
                "LispSyntax",
                MemoryInfo(sizeof(LispSyntax), sizeof(LispSyntax))
                );
            return theType;
        }
    };

    class LispSyntax_Builtin :
        public LispSyntax
    {
    public:
        typedef Ptr<LispObject>(*HandlerFuncPtr_t)(Ptr<LispRuntime>, Ptr<Environment>, Ptr<LispObject>);

        LispSyntax_Builtin(Ptr<LispSymbol> pName,
                             HandlerFuncPtr_t pHandler);

        virtual Ptr<LispObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;

        virtual Ptr<LispObject> call(Ptr<LispRuntime> pRuntime,
                                       Ptr<Environment> pEnv,
                                       Ptr<LispObject> pUnevaluatedArgList) LCPP_OVERRIDE;

    private:
        HandlerFuncPtr_t m_pHandler;
    };
}

#include "lcpp/core/typeSystem/types/implementation/syntax_inl.h"
