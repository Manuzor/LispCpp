#include "stdafx.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/runtime.h"

lcpp::TypeFactory::TypeFactory() :
    m_symbols(LispRuntime::instance()->allocator().get()),
    m_integers(LispRuntime::instance()->allocator().get())
{
}

lcpp::TypeFactory::~TypeFactory()
{
    m_integers.Clear();
    m_symbols.Clear();
}

lcpp::Ptr<lcpp::LispSymbol>
lcpp::TypeFactory::createSymbol(const ezString& symbol)
{
    Ptr<LispSymbol> pResult;
    if (!m_symbols.TryGetValue(symbol, pResult))
    {
        pResult = LCPP_NEW(LispRuntime::instance()->allocator().get(), LispSymbol)(symbol);
        m_symbols[symbol] = pResult;
    }
    EZ_ASSERT(pResult, "The result should never be a nullptr!");

    return pResult;
}

lcpp::Ptr<lcpp::LispCons>
lcpp::TypeFactory::createCons(Ptr<LispObject> pCar, Ptr<LispObject> pCdr)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), LispCons)(pCar, pCdr);
}

lcpp::Ptr<lcpp::LispFile>
lcpp::TypeFactory::createFile(const ezString& fileName)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), LispFile)(fileName);
}

lcpp::Ptr<lcpp::LispFunction>
lcpp::TypeFactory::createUserDefinedFunction(Ptr<Environment> pParentEnv,
                                             Ptr<LispObject> pArgNameList,
                                             Ptr<LispCons> pBody)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), LispFunctionUserDefined)(Environment::create(pParentEnv), pArgNameList, pBody);
}

lcpp::Ptr<lcpp::LispFunction>
lcpp::TypeFactory::createBuiltinFunction(const ezString& name,
                                         Ptr<Environment> pParentEnv,
                                         LispFunctionBuiltin::ExecutorPtr_t executor)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), LispFunctionBuiltin)(name, Environment::create(pParentEnv), executor);
}

lcpp::Ptr<lcpp::LispSyntax>
lcpp::TypeFactory::createSyntax_Builtin(Ptr<LispSymbol> pName,
                                        LispSyntax_Builtin::HandlerFuncPtr_t pHandler)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), LispSyntax_Builtin)(pName, pHandler);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::TypeFactory::copy(Ptr<LispObject> pObject)
{
    if(pObject->is<LispSymbol>())        { return copy(pObject.cast<LispSymbol>());   }
    if(pObject->is<LispCons>())          { return copy(pObject.cast<LispCons>());     }
    if(pObject->is<LispFile>())          { return copy(pObject.cast<LispFile>());     }
    if(pObject->is<LispFunction>())      { return copy(pObject.cast<LispFunction>()); }
    if(pObject->is<LispSyntax>())        { return copy(pObject.cast<LispSyntax>()); }

    if(pObject->is<LispNil>()) { return copy(pObject.cast<LispNil>()); }
    if(pObject->is<LispBool>()) { return copy(pObject.cast<LispBool>()); }
    if(pObject->is<LispVoid>()) { return copy(pObject.cast<LispVoid>()); }

    if(pObject->is<LispSyntax>()){ return pObject; }
    
    throw exceptions::NotImplemented("Unsupported type to copy!");
}

lcpp::Ptr<lcpp::LispSymbol>
lcpp::TypeFactory::copy(Ptr<LispSymbol> pSymbol)
{
    return createSymbol(pSymbol->value());
}

lcpp::Ptr<lcpp::LispCons>
lcpp::TypeFactory::copy(Ptr<LispCons> pCons)
{
    return createCons(copy(pCons->car()),
                      copy(pCons->cdr()));
}

lcpp::Ptr<lcpp::LispFile>
lcpp::TypeFactory::copy(Ptr<LispFile> pFile)
{
    return createFile(pFile->name());
}


lcpp::Ptr<lcpp::LispFunction>
lcpp::TypeFactory::copy(Ptr<LispFunction> pFunc)
{
    return pFunc->clone(LispRuntime::instance()->allocator().get());
}

lcpp::Ptr<lcpp::LispSyntax>
lcpp::TypeFactory::copy(Ptr<LispSyntax> pSyntax)
{
    return pSyntax.cast<LispObject>()->clone(LispRuntime::instance()->allocator().get());
}
