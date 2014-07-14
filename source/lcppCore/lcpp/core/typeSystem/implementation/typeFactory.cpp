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

lcpp::Ptr<lcpp::LispObject>
lcpp::TypeFactory::copy(Ptr<LispObject> pObject)
{
    if(pObject->is<LispNil>()) { return copy(pObject.cast<LispNil>()); }
    if(pObject->is<LispBool>()) { return copy(pObject.cast<LispBool>()); }
    if(pObject->is<LispVoid>()) { return copy(pObject.cast<LispVoid>()); }

    if(pObject->is<LispSyntax>()){ return pObject; }
    
    throw exceptions::NotImplemented("Unsupported type to copy!");
}
