#pragma once
#include "lcpp/core/typeSystem/types/streamCommon.h"

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace stream
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT StackPtr<LispObject> create(ezStringIterator& iterator);

        LCPP_API_CORE_CONT ezStringIterator& getIterator(StackPtr<LispObject> pStream);
        LCPP_API_CORE_CONT void setIterator(StackPtr<LispObject> pStream, ezStringIterator& iter);

        LCPP_API_CORE_CONT ezUInt32 getCharacter(StackPtr<LispObject> pStream);

        LCPP_API_CORE_CONT bool isValid(StackPtr<LispObject> pStream);

        LCPP_API_CORE_CONT ezUInt32 next(StackPtr<LispObject> pStream);

        LCPP_API_CORE_CONT ezUInt32 getPosition(StackPtr<LispObject> pStream);

        LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pObject);
    };
}
