#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace nil
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create();

        LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pObject);

    };

    bool isNil(StackPtr<LispObject> pObject);
}

#define LCPP_pNil ::lcpp::nil::create()

#include "lcpp/core/typeSystem/types/impl/nil.inl"
