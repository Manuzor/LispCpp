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

        Ptr<LispCons>     createCons(Ptr<LispObject> car, Ptr<LispObject> cdr);
        Ptr<LispFile>     createFile(const ezString& fileName);
        Ptr<LispFunction> createUserDefinedFunction(Ptr<Environment> pParentEnv,
                                                    Ptr<LispObject> pArgNameList,
                                                    Ptr<LispCons> pBody);
        Ptr<LispFunction> createBuiltinFunction(const ezString& name,
                                                Ptr<Environment> pParentEnv,
                                                LispFunctionBuiltin::ExecutorPtr_t executor);
        Ptr<LispSyntax> createSyntax_Builtin(Ptr<LispSymbol> pName,
                                             LispSyntax_Builtin::HandlerFuncPtr_t pHandler);

        // General copy function
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> copy(Ptr<LispObject> pObject);

        // Specialized copy function for each type
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispCons>     copy(Ptr<LispCons> pCons);
        Ptr<LispFile>     copy(Ptr<LispFile> pFile);
        Ptr<LispFunction> copy(Ptr<LispFunction> pFunc);
        Ptr<LispSyntax>   copy(Ptr<LispSyntax> pSyntax);

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
