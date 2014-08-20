#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace str
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(const String& value);

        LCPP_API_CORE_CONT const String& getValue(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

    }
}
