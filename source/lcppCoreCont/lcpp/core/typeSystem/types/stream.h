#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace stream
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(ezStringIterator& iterator);

        //////////////////////////////////////////////////////////////////////////

        enum
        {
            EndOfStream = ezUInt32(-1),
        };
        
        class Data
        {
        public:

            ezStringIterator& getIterator();

        public:

            LCPP_DeclareRawDataMember(ezStringIterator, m_iterator);
        };

        //////////////////////////////////////////////////////////////////////////

        LCPP_API_CORE_CONT ezStringIterator& getIterator(Ptr<LispObject> pStream);

        LCPP_API_CORE_CONT ezUInt32 getCharacter(Ptr<LispObject> pStream);

        LCPP_API_CORE_CONT bool isValid(Ptr<LispObject> pStream);

        LCPP_API_CORE_CONT ezUInt32 next(Ptr<LispObject> pStream);

        LCPP_API_CORE_CONT ezUInt32 getPosition(Ptr<LispObject> pStream);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);
    };
}

#include "lcpp/core/typeSystem/types/impl/stream.inl"
