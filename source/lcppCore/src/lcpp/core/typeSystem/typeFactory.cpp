#include "stdafx.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/runtime.h"

lcpp::TypeFactory::TypeFactory(Ptr<SchemeRuntime> pRuntime) :
    m_pRuntime(pRuntime),
    m_symbols(pRuntime->allocator().get()),
    m_integers(pRuntime->allocator().get())
{
    EZ_ASSERT(pRuntime, "Invalid runtime!");
}

lcpp::TypeFactory::~TypeFactory()
{
    m_symbols.Clear();
    m_integers.Clear();
}

lcpp::Ptr<lcpp::Environment>
lcpp::TypeFactory::createEnvironment(const ezString& name, Ptr<Environment> pParent)
{
    return LCPP_NEW(m_pRuntime->allocator().get(), Environment)(name, pParent);
}

lcpp::Ptr<lcpp::SchemeInteger>
lcpp::TypeFactory::createInteger(SchemeInteger::Number_t value)
{
    Ptr<SchemeInteger> pResult;
    if(!m_integers.TryGetValue(value, pResult))
    {
        pResult = LCPP_NEW(m_pRuntime->allocator().get(), SchemeInteger)(value);
        m_integers[value] = pResult;
    }
    EZ_ASSERT(pResult, "The result should never be a nullptr!");

    return pResult;
}

lcpp::Ptr<lcpp::SchemeNumber>
lcpp::TypeFactory::createNumber(SchemeNumber::Number_t value)
{
    return LCPP_NEW(m_pRuntime->allocator().get(), SchemeNumber)(value);
}

lcpp::Ptr<lcpp::SchemeString>
lcpp::TypeFactory::createString(const ezString& str)
{
    return LCPP_NEW(m_pRuntime->allocator().get(), SchemeString)(str);
}

lcpp::Ptr<lcpp::SchemeSymbol>
lcpp::TypeFactory::createSymbol(const ezString& symbol)
{
    Ptr<SchemeSymbol> pResult;
    if (!m_symbols.TryGetValue(symbol, pResult))
    {
        pResult = LCPP_NEW(m_pRuntime->allocator().get(), SchemeSymbol)(symbol);
        m_symbols[symbol] = pResult;
    }
    EZ_ASSERT(pResult, "The result should never be a nullptr!");

    return pResult;
}

lcpp::Ptr<lcpp::SchemeCons>
lcpp::TypeFactory::createCons(Ptr<SchemeObject> pCar, Ptr<SchemeObject> pCdr)
{
    return LCPP_NEW(m_pRuntime->allocator().get(), SchemeCons)(pCar, pCdr);
}

lcpp::Ptr<lcpp::SchemeFile>
lcpp::TypeFactory::createFile(const ezString& fileName)
{
    return LCPP_NEW(m_pRuntime->allocator().get(), SchemeFile)(fileName);
}

lcpp::Ptr<lcpp::SchemeFunction>
lcpp::TypeFactory::createUserDefinedFunction(Ptr<Environment> pParentEnv,
                                             Ptr<SchemeObject> pArgNameList,
                                             Ptr<SchemeCons> pBody)
{
    return LCPP_NEW(m_pRuntime->allocator().get(), SchemeFunctionUserDefined)(m_pRuntime, createEnvironment("", pParentEnv), pArgNameList, pBody);
}

lcpp::Ptr<lcpp::SchemeFunction>
lcpp::TypeFactory::createBuiltinFunction(const ezString& name,
                                         Ptr<Environment> pParentEnv,
                                         SchemeFunctionBuiltin::ExecutorPtr_t executor)
{
    return LCPP_NEW(m_pRuntime->allocator().get(), SchemeFunctionBuiltin)(m_pRuntime, name, createEnvironment("", pParentEnv), executor);
}

lcpp::Ptr<lcpp::SchemeSyntax>
lcpp::TypeFactory::createSyntax_Builtin(Ptr<SchemeSymbol> pName,
                                        Ptr<SchemeCons> pUnevaluatedArgList,
                                        SchemeSyntax_Builtin::HandlerFuncPtr_t pHandler)
{
    return LCPP_NEW(m_pRuntime->allocator().get(), SchemeSyntax_Builtin)(pName, pUnevaluatedArgList, pHandler);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::TypeFactory::copy(Ptr<SchemeObject> pObject)
{
    if(pObject->is<SchemeInteger>())  { return copy(pObject.cast<SchemeInteger>());  }
    if(pObject->is<SchemeNumber>())   { return copy(pObject.cast<SchemeNumber>());   }
    if(pObject->is<SchemeString>())   { return copy(pObject.cast<SchemeString>());   }
    if(pObject->is<SchemeSymbol>())   { return copy(pObject.cast<SchemeSymbol>());   }
    if(pObject->is<SchemeCons>())     { return copy(pObject.cast<SchemeCons>());     }
    if(pObject->is<SchemeFile>())     { return copy(pObject.cast<SchemeFile>());     }
    if(pObject->is<SchemeFunction>()) { return copy(pObject.cast<SchemeFunction>()); }
    if(pObject->is<SchemeSyntax>())   { return copy(pObject.cast<SchemeSyntax>());   }

    if(pObject->is<SchemeNil>()) { return copy(pObject.cast<SchemeNil>()); }
    if(pObject->is<SchemeBool>()) { return copy(pObject.cast<SchemeBool>()); }
    if(pObject->is<SchemeVoid>()) { return copy(pObject.cast<SchemeVoid>()); }

    if(pObject->is<SchemeSyntax>()){ return pObject; }
    
    throw exceptions::NotImplemented("Unsupported type to copy!");
}

lcpp::Ptr<lcpp::SchemeInteger> lcpp::TypeFactory::copy(Ptr<SchemeInteger> pInteger)
{
    return createInteger(pInteger->value());
}

lcpp::Ptr<lcpp::SchemeNumber>
lcpp::TypeFactory::copy(Ptr<SchemeNumber> pNumber)
{
    return createNumber(pNumber->value());
}

lcpp::Ptr<lcpp::SchemeString>
lcpp::TypeFactory::copy(Ptr<SchemeString> pString)
{
    return createString(pString->value());
}

lcpp::Ptr<lcpp::SchemeSymbol>
lcpp::TypeFactory::copy(Ptr<SchemeSymbol> pSymbol)
{
    return createSymbol(pSymbol->value());
}

lcpp::Ptr<lcpp::SchemeCons>
lcpp::TypeFactory::copy(Ptr<SchemeCons> pCons)
{
    return createCons(copy(pCons->car()),
                      copy(pCons->cdr()));
}

lcpp::Ptr<lcpp::SchemeFile>
lcpp::TypeFactory::copy(Ptr<SchemeFile> pFile)
{
    return createFile(pFile->name());
}


lcpp::Ptr<lcpp::SchemeFunction>
lcpp::TypeFactory::copy(Ptr<SchemeFunction> pFunc)
{
    return pFunc->clone(m_pRuntime->allocator().get());
}

lcpp::Ptr<lcpp::SchemeSyntax>
lcpp::TypeFactory::copy(Ptr<SchemeSyntax> pSyntax)
{
    return pSyntax->clone(m_pRuntime->allocator().get());
}
