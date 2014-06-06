#pragma once
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/function.h"
#include "lcpp/core/typeSystem/types/syntax.h"

namespace lcpp
{
    class SchemeNil;
    class SchemeBool;
    class SchemeVoid;

    class SchemeObject;
    class SchemeCons;
    class SchemeString;
    class SchemeSymbol;
    class SchemeFile;

    class Environment;
    class IEvaluator;

    class LCPP_CORE_API TypeFactory
    {
    public:
        TypeFactory(ezAllocatorBase* pAllocator = defaultAllocator());
        ~TypeFactory();

        Ptr<Environment>    createEnvironment(const ezString& name, Ptr<Environment> pParent);

        Ptr<SchemeInteger>  createInteger(SchemeInteger::Number_t value);
        Ptr<SchemeNumber>   createNumber(SchemeNumber::Number_t value);
        Ptr<SchemeString>   createString(const ezString& str);
        Ptr<SchemeSymbol>   createSymbol(const ezString& symbol);
        Ptr<SchemeCons>     createCons(Ptr<SchemeObject> car, Ptr<SchemeObject> cdr);
        Ptr<SchemeFile>     createFile(const ezString& fileName);
        Ptr<SchemeFunction> createUserDefinedFunction(Ptr<Environment> pParentEnv,
                                                      Ptr<SchemeObject> pArgNameList,
                                                      Ptr<SchemeCons> pBody);
        Ptr<SchemeFunction> createBuiltinFunction(const ezString& name,
                                                  Ptr<Environment> pParentEnv,
                                                  SchemeFunctionBuiltin::Executor executor);
        Ptr<SchemeSyntax>   createSyntax(Ptr<SchemeSymbol> pName,
                                         Ptr<SchemeCons> pUnevaluatedArgList,
                                         SchemeSyntax::HandlerFuncPtr_t pHandler);

        // General copy function
        //////////////////////////////////////////////////////////////////////////
        Ptr<SchemeObject> copy(Ptr<SchemeObject> pObject);

        // Specialized copy function for each type
        //////////////////////////////////////////////////////////////////////////
        Ptr<SchemeInteger>  copy(Ptr<SchemeInteger> pInteger);
        Ptr<SchemeNumber>   copy(Ptr<SchemeNumber> pNumber);
        Ptr<SchemeString>   copy(Ptr<SchemeString> pString);
        Ptr<SchemeSymbol>   copy(Ptr<SchemeSymbol> pSymbol);
        Ptr<SchemeCons>     copy(Ptr<SchemeCons> pCons);
        Ptr<SchemeFile>     copy(Ptr<SchemeFile> pFile);
        Ptr<SchemeFunction> copy(Ptr<SchemeFunction> pFunc);

        // Singletons
        //////////////////////////////////////////////////////////////////////////
        Ptr<SchemeNil>  copy(Ptr<SchemeNil> pNil) { return pNil; }
        Ptr<SchemeBool> copy(Ptr<SchemeBool> pBool) { return pBool; }
        Ptr<SchemeVoid> copy(Ptr<SchemeVoid> pVoid) { return pVoid; }

    private:
        ezAllocatorBase* m_pAllocator;

        ezHashTable<ezString, Ptr<SchemeSymbol>> m_symbols;
        ezHashTable<SchemeInteger::Number_t, Ptr<SchemeInteger>> m_integers;
    };
}
