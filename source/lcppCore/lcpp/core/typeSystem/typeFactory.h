#pragma once
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/function.h"
#include "lcpp/core/typeSystem/types/syntax.h"

namespace lcpp
{
    class LispNil;
    class LispBool;
    class LispVoid;

    class LispObject;
    class LispCons;
    class LispString;
    class LispSymbol;
    class LispFile;

    class LispRuntime;
    class Environment;
    class IEvaluator;

    class LCPP_CORE_API TypeFactory
    {
        friend class LispRuntime;
    public:
        ~TypeFactory();

        // General copy function
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> copy(Ptr<LispObject> pObject);

        // Singletons
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispNil>  copy(Ptr<LispNil> pNil) { return pNil; }
        Ptr<LispBool> copy(Ptr<LispBool> pBool) { return pBool; }
        Ptr<LispVoid> copy(Ptr<LispVoid> pVoid) { return pVoid; }

    private:
        ezHashTable<ezString, Ptr<LispSymbol>> m_symbols;
        ezHashTable<LispInteger::Number_t, Ptr<LispInteger>> m_integers;

    private:
        TypeFactory();
    };
}
