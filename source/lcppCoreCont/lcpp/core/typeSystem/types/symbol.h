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

            void initialize(String* pSymbol);
            void destroy();

            String* m_pValue;
        };

        LCPP_API_CORE_CONT const String& value(ezInt32 index = -1);

    }
}

#include "lcpp/core/typeSystem/types/impl/symbol.inl"
