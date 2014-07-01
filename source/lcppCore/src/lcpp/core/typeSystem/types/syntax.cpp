#include "stdafx.h"

#include "lcpp/core/typeSystem.h"
#include "lcpp/core/evaluator.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::SchemeSyntax::SchemeSyntax(Ptr<SchemeSymbol> pName,
                                 Ptr<SchemeCons> pUnevaluatedArgList) :
    m_pName(pName),
    m_pUnevaluatedArgList(pUnevaluatedArgList)
{
    EZ_ASSERT(m_pUnevaluatedArgList, "Invalid cons!");
}

bool
lcpp::SchemeSyntax::operator==(const SchemeObject& obj) const
{
    if (obj.is<SchemeSyntax>())
    {
        return *this == static_cast<const SchemeSyntax&>(obj);
    }
    return false;
}

bool
lcpp::SchemeSyntax::operator==(const SchemeSyntax& rhs) const
{
    // Identity comparison.
    return m_pName.get() == rhs.m_pName.get();
}

ezString
lcpp::SchemeSyntax::toString() const
{
    return m_pName->toString();
}
lcpp::SchemeSyntax_Builtin::SchemeSyntax_Builtin(Ptr<SchemeSymbol> pName,
                                                 Ptr<SchemeCons> pUnevaluatedArgList,
                                                 HandlerFuncPtr_t pHandler) :
    SchemeSyntax(pName,
                 pUnevaluatedArgList),
    m_pHandler(pHandler)
{
    EZ_ASSERT(m_pHandler, "Invalid function pointer!");
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeSyntax_Builtin::call(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv)
{
    return (*m_pHandler)(pRuntime, pEnv, m_pUnevaluatedArgList);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeSyntax_Builtin::clone(ezAllocatorBase* pAllocator) const
{
    return LCPP_NEW(pAllocator, SchemeSyntax_Builtin)(m_pName, m_pUnevaluatedArgList, m_pHandler);
}
