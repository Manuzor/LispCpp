#pragma once
#include "lcpp/core/typeSystem/types/streamCommon.h"
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace stream
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        //LCPP_API_CORE_CONT Ptr<LispObject> create(ezStringIterator& iterator);
        LCPP_API_CORE_CONT Ptr<LispObject> create(StackPtr<LispObject> pString);

        inline Ptr<LispObject> create(Ptr<LispObject> pString)
        {
            StackPtr<LispObject> pArg(pString);
            return create(pArg);
        }

        template<typename T>
        Ptr<LispObject> create(const T& theString) { return create(str::create(theString.GetData(), theString.GetElementCount())); }

        LCPP_API_CORE_CONT ezStringIterator& getIterator(Ptr<LispObject> pStream);
        LCPP_API_CORE_CONT void setIterator(Ptr<LispObject> pStream, ezStringIterator& iter);

        LCPP_API_CORE_CONT ezUInt32 getCharacter(Ptr<LispObject> pStream);

        LCPP_API_CORE_CONT bool isValid(Ptr<LispObject> pStream);

        LCPP_API_CORE_CONT ezUInt32 next(Ptr<LispObject> pStream);

        LCPP_API_CORE_CONT ezUInt32 getPosition(Ptr<LispObject> pStream);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(StackPtr<LispObject> pObject);
    };
}
