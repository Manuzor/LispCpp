#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/typeSystem/types/cons.h"

namespace lcpp
{
    class SchemeFunction : public SchemeExtend<SchemeFunction, SchemeObject>
    {
        friend class TypeFactory;
    public:
        SchemeFunction(Ptr<Environment> pEnv, Ptr<SchemeCons> pBody);

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeFunction& rhs) const;

        virtual ezString toString() const LCPP_OVERRIDE;

    private:
        Ptr<Environment> m_pEnv;
        Ptr<SchemeCons> m_pBody;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeFunction);
}

#include "lcpp/core/typeSystem/types/implementation/function.inl"
