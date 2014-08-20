#pragma once

namespace lcpp
{
    namespace str
    {
        class Data
        {
        public:

            const String& getValue() const;
            String& getValue();
            void setValue(const String& newString);

            LCPP_DeclareRawDataMember(String, m_pRawData);
        };
    }
}

#include "lcpp/core/typeSystem/types/impl/stringData.inl"
