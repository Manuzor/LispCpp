#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace symbol
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(const String& value);

        class Data
        {
        public:

            void initialize(String* pSymbol);
            void deinitialize();

            String* m_pValue;
        };

        LCPP_API_CORE_CONT const String& value(ezInt32 index = -1);

    }
}

#include "lcpp/core/typeSystem/types/impl/symbol.inl"
