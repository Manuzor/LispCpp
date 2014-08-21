#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace nil
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create();

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

    };

    bool isNil(Ptr<LispObject> pObject);
}

#define LCPP_pNil ::lcpp::nil::create()

#include "lcpp/core/typeSystem/types/impl/nil.inl"
