#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace symbol
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(const String& value);

        /// \brief Creates an actual new instance, without consulting the symbol table first.
        LCPP_API_CORE_CONT Ptr<LispObject> createNew(const String& value);

        class Data
        {
        public:

            void initialize(const String& symbol);
            void destroy();

            const String& getString() const;
            String& getString();
            void setString(const String& newString);

            char m_pRawData[sizeof(String)];
        };

        LCPP_API_CORE_CONT const String& value(ezInt32 index = -1);

    }
}

#include "lcpp/core/typeSystem/types/impl/symbol.inl"
