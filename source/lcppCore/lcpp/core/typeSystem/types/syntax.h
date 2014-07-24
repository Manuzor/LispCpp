#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispRuntime;
    class LispEnvironment;
    class LispSymbol;
    class LispCons;

    class LCPP_CORE_API LispSyntax :
        public LispObject
    {
    public:

        static Ptr<LispSyntax> create(Ptr<LispSymbol> pName);
        static const Type& typeInfo();

    public:

        LispSyntax(Ptr<LispSymbol> pName);

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual bool operator==(const LispObject& obj) const LCPP_OVERRIDE;
        bool operator==(const LispSyntax& rhs) const;

        virtual String toString() const LCPP_OVERRIDE;

        void lcpp::LispSyntax::name(lcpp::Ptr<lcpp::LispSymbol> pName);
        lcpp::Ptr<      lcpp::LispSymbol> lcpp::LispSyntax::name();
        lcpp::Ptr<const lcpp::LispSymbol> lcpp::LispSyntax::name() const;

        virtual Ptr<LispObject> call(Ptr<LispEnvironment> pEnv,
                                     Ptr<LispObject> pUnevaluatedArgList) = 0;

    protected:
        Ptr<LispSymbol> m_pName;
    };

    class LispSyntax_BuiltIn :
        public LispSyntax
    {
    public:
        typedef Ptr<LispObject>(*HandlerFuncPtr_t)(Ptr<LispEnvironment>, Ptr<LispObject>);


        static Ptr<LispSyntax_BuiltIn> create(Ptr<LispSymbol> pName,
                                              HandlerFuncPtr_t pHandler);
        static const Type& typeInfo();

    public:

        LispSyntax_BuiltIn(Ptr<LispSymbol> pName,
                             HandlerFuncPtr_t pHandler);

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;

        virtual Ptr<LispObject> call(Ptr<LispEnvironment> pEnv,
                                     Ptr<LispObject> pUnevaluatedArgList) LCPP_OVERRIDE;

    private:
        HandlerFuncPtr_t m_pHandler;
    };
}

#include "lcpp/core/typeSystem/types/implementation/syntax.inl"
