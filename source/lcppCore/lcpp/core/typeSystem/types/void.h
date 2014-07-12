#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispVoid :
        public LispExtend<LispVoid, LispObject>
    {
        friend class TypeFactory;
    public:

        static LispVoid& instance();

        virtual ~LispVoid();

        virtual bool operator==(const LispObject& obj) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;
    private:

        LispVoid();
    };

    LCPP_DECLARE_SCHEME_TYPE_INFO(LispVoid, "void");
}

#define SCHEME_VOID (::lcpp::LispVoid::instance())
#define SCHEME_VOID_PTR (Ptr<LispVoid>(&SCHEME_VOID))

#include "lcpp/core/typeSystem/types/implementation/void_inl.h"
