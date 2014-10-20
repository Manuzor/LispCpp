#pragma once

namespace lcpp
{
    class MetaInfo;
    class LispObject;

    namespace time
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create();
        LCPP_API_CORE_CONT Ptr<LispObject> create(const ezTime& theTime);

        LCPP_API_CORE_CONT ezTime& getTime(Ptr<LispObject> pTime);
        LCPP_API_CORE_CONT void setTime(Ptr<LispObject> pTime, const ezTime& theTime);
        LCPP_API_CORE_CONT void setNow(Ptr<LispObject> pTime);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(StackPtr<LispObject> pTime);
    }
}

#include "lcpp/core/typeSystem/types/impl/time.inl"
