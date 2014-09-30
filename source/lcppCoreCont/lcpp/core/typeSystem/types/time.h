#pragma once

namespace lcpp
{
    class MetaInfo;
    class LispObject;

    namespace time
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT StackPtr<LispObject> create();
        LCPP_API_CORE_CONT StackPtr<LispObject> create(const ezTime& theTime);

        LCPP_API_CORE_CONT ezTime& getTime(StackPtr<LispObject> pTime);
        LCPP_API_CORE_CONT void setTime(StackPtr<LispObject> pTime, const ezTime& theTime);
        LCPP_API_CORE_CONT void setNow(StackPtr<LispObject> pTime);

        LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pTime);
    }
}

#include "lcpp/core/typeSystem/types/impl/time.inl"
