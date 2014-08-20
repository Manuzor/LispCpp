#pragma once
#include "lcpp/core/typeSystem/types/streamCommon.h"

namespace lcpp
{
    namespace stream
    {
        class Data
        {
        public:

            ezStringIterator& getIterator();

        public:

            LCPP_DeclareRawDataMember(ezStringIterator, m_iterator);
        };
    };
}

#include "lcpp/core/typeSystem/types/impl/streamData.inl"
