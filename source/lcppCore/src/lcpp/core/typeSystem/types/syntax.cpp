#include "stdafx.h"

#include "lcpp/core/typeSystem.h"
#include "lcpp/core/evaluator.h"

lcpp::SchemeSyntax::SchemeSyntax(Ptr<SchemeSymbol> pName,
                                 Ptr<SchemeCons> pUnevaluatedArgList,
                                 HandlerFuncPtr_t pHandler) :
    m_pName(pName),
    m_pUnevaluatedArgList(pUnevaluatedArgList),
    m_pHandler(pHandler)
{
    EZ_ASSERT(m_pUnevaluatedArgList, "Invalid cons!");
    EZ_ASSERT(m_pHandler, "Invalid function pointer!");
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
    ezStringBuilder builder;
    builder.AppendFormat("<syntax:%s>", m_pName->toString().GetData());
    return builder;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeSyntax::call(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv)
{
    return (*m_pHandler)(pRuntime, pEnv, m_pUnevaluatedArgList);
}
