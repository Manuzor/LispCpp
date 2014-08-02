#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace str
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(const String& value);

        //////////////////////////////////////////////////////////////////////////

        class Data
        {
        public:

            const String& getValue() const;
            String& getValue();
            void setValue(const String& newString);

            LCPP_DeclareRawDataMember(String, m_pRawData);
        };

        //////////////////////////////////////////////////////////////////////////

        LCPP_API_CORE_CONT const String& getValue(Ptr<LispObject> pObject);

    }
}

#include "lcpp/core/typeSystem/types/impl/string.inl"
