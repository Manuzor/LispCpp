#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace symbol
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(const String& value);
        Ptr<LispObject> create(ezUInt32 character);

        /// \brief Creates an actual new instance, without consulting the symbol table first.
        LCPP_API_CORE_CONT Ptr<LispObject> createNew(const String& value);

        LCPP_API_CORE_CONT const String& getValue(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

    }
}

#include "lcpp/core/typeSystem/types/impl/symbol.inl"
