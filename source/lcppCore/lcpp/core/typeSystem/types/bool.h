#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispBool :
        public LispExtend<LispBool, LispObject>
    {
        friend class TypeFactory;
    public:

        static LispBool& trueInstance();
        static LispBool& falseInstance();

        virtual ~LispBool();

        virtual bool operator==(const LispObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        bool value() const;

    private:
        const bool m_value;

        LispBool(bool value);
    };

    LCPP_DECLARE_SCHEME_TYPE_INFO(LispBool, "bool");

    bool isTrue(Ptr<LispObject> pObject);
    bool isFalse(Ptr<LispObject> pObject);

    bool operator ==(const LispBool& lhs, const LispBool& rhs);

#define SCHEME_TRUE (::lcpp::LispBool::trueInstance())
#define SCHEME_TRUE_PTR (Ptr<LispBool>(&SCHEME_TRUE))
#define SCHEME_FALSE (::lcpp::LispBool::falseInstance())
#define SCHEME_FALSE_PTR (Ptr<LispBool>(&SCHEME_FALSE))

}

#include "lcpp/core/typeSystem/types/implementation/bool_inl.h"
