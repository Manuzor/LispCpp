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

        //////////////////////////////////////////////////////////////////////////

        class Data
        {
        public:

            const String& getValue() const;
            String& getValue();
            void setValue(const String& newString);

            char m_pRawData[sizeof(String)];
        };

        //////////////////////////////////////////////////////////////////////////

        LCPP_API_CORE_CONT const String& value(Ptr<LispObject> pObject);

    }
}

#include "lcpp/core/typeSystem/types/impl/symbol.inl"
