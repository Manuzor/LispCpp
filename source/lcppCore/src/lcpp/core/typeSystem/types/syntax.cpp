#include "stdafx.h"

#include "lcpp/foundation/ptr.h"

#include "lcpp/core/typeSystem.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/recursionCounter.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

//////////////////////////////////////////////////////////////////////////

lcpp::SchemeSyntax::SchemeSyntax(Ptr<SchemeSymbol> pName) :
    m_pName(pName)
{
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

//////////////////////////////////////////////////////////////////////////

lcpp::SchemeSyntax_Builtin::SchemeSyntax_Builtin(Ptr<SchemeSymbol> pName,
                                                 HandlerFuncPtr_t pHandler) :
    SchemeSyntax(pName),
    m_pHandler(pHandler)
{
    EZ_ASSERT(m_pHandler, "Invalid syntax handler pointer!");
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeSyntax_Builtin::call(Ptr<SchemeRuntime> pRuntime,
                                 Ptr<Environment> pEnv,
                                 Ptr<SchemeObject> pUnevaluatedArgList)
{
    RecursionCounter counter(pRuntime);
    auto& handler = *m_pHandler;
    return (*m_pHandler)(pRuntime, pEnv, pUnevaluatedArgList);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeSyntax_Builtin::clone(ezAllocatorBase* pAllocator) const
{
    return LCPP_NEW(pAllocator, SchemeSyntax_Builtin)(m_pName, m_pHandler);
}
