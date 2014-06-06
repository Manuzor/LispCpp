#include "stdafx.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/environment.h"

lcpp::TypeFactory::TypeFactory(ezAllocatorBase* pAllocator) :
    m_pAllocator(pAllocator),
    m_symbols(pAllocator),
    m_integers(pAllocator)
{
}

lcpp::TypeFactory::~TypeFactory()
{
    m_symbols.Clear();
    m_integers.Clear();
}

lcpp::Ptr<lcpp::Environment>
lcpp::TypeFactory::createEnvironment(const ezString& name, Ptr<Environment> pParent)
{
    return LCPP_NEW(m_pAllocator, Environment)(name, pParent);
}

lcpp::Ptr<lcpp::SchemeInteger>
lcpp::TypeFactory::createInteger(SchemeInteger::Number_t value)
{
    Ptr<SchemeInteger> pResult;
    if(!m_integers.TryGetValue(value, pResult))
    {
        pResult = LCPP_NEW(m_pAllocator, SchemeInteger)(value);
        m_integers[value] = pResult;
    }
    EZ_ASSERT(pResult, "The result should never be a nullptr!");

    return pResult;
}

lcpp::Ptr<lcpp::SchemeNumber>
lcpp::TypeFactory::createNumber(SchemeNumber::Number_t value)
{
    return LCPP_NEW(m_pAllocator, SchemeNumber)(value);
}

lcpp::Ptr<lcpp::SchemeString>
lcpp::TypeFactory::createString(const ezString& str)
{
    return LCPP_NEW(m_pAllocator, SchemeString)(str);
}

lcpp::Ptr<lcpp::SchemeSymbol>
lcpp::TypeFactory::createSymbol(const ezString& symbol)
{
    Ptr<SchemeSymbol> pResult;
    if (!m_symbols.TryGetValue(symbol, pResult))
    {
        pResult = LCPP_NEW(m_pAllocator, SchemeSymbol)(symbol);
        m_symbols[symbol] = pResult;
    }
    EZ_ASSERT(pResult, "The result should never be a nullptr!");

    return pResult;
}

lcpp::Ptr<lcpp::SchemeCons>
lcpp::TypeFactory::createCons(Ptr<SchemeObject> pCar, Ptr<SchemeObject> pCdr)
{
    return LCPP_NEW(m_pAllocator, SchemeCons)(pCar, pCdr);
}

lcpp::Ptr<lcpp::SchemeFile>
lcpp::TypeFactory::createFile(const ezString& fileName)
{
    return LCPP_NEW(m_pAllocator, SchemeFile)(fileName);
}

lcpp::Ptr<lcpp::SchemeFunction>
lcpp::TypeFactory::createUserDefinedFunction(Ptr<Environment> pParentEnv,
                                             Ptr<SchemeObject> pArgNameList,
                                             Ptr<SchemeCons> pBody)
{
    return LCPP_NEW(m_pAllocator, SchemeFunctionUserDefined)(createEnvironment("", pParentEnv), pArgNameList, pBody);
}

lcpp::Ptr<lcpp::SchemeFunction>
lcpp::TypeFactory::createBuiltinFunction(const ezString& name,
                                         Ptr<Environment> pParentEnv,
                                         SchemeFunctionBuiltin::Executor executor)
{
    return LCPP_NEW(m_pAllocator, SchemeFunctionBuiltin)(name, createEnvironment("", pParentEnv), executor);
}

lcpp::Ptr<lcpp::SchemeSyntax>
lcpp::TypeFactory::createSyntax(Ptr<SchemeSymbol> pName,
                                Ptr<SchemeCons> pUnevaluatedArgList,
                                SchemeSyntax::HandlerFuncPtr_t pHandler)
{
    return LCPP_NEW(m_pAllocator, SchemeSyntax)(pName, pUnevaluatedArgList, pHandler);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::TypeFactory::copy(Ptr<SchemeObject> pObject)
{
    if(pObject->is<SchemeInteger>())  { return copy(pObject.cast<SchemeInteger>()); }
    if(pObject->is<SchemeNumber>())   { return copy(pObject.cast<SchemeNumber>()); }
    if(pObject->is<SchemeString>())   { return copy(pObject.cast<SchemeString>()); }
    if(pObject->is<SchemeSymbol>())   { return copy(pObject.cast<SchemeSymbol>()); }
    if(pObject->is<SchemeCons>())     { return copy(pObject.cast<SchemeCons>()); }
    if(pObject->is<SchemeFile>())     { return copy(pObject.cast<SchemeFile>()); }
    if(pObject->is<SchemeFunction>()) { return copy(pObject.cast<SchemeFunction>()); }

    if(pObject->is<SchemeNil>()) { return copy(pObject.cast<SchemeNil>()); }
    if(pObject->is<SchemeBool>()) { return copy(pObject.cast<SchemeBool>()); }
    if(pObject->is<SchemeVoid>()) { return copy(pObject.cast<SchemeVoid>()); }
    
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
    return pFunc->clone(m_pAllocator);
}
