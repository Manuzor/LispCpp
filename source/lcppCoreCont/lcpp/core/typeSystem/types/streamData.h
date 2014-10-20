#pragma once
#include "lcpp/core/typeSystem/types/streamCommon.h"

namespace lcpp
{
    namespace stream
    {
        class Data
        {
        public:
            Ptr<LispObject> m_pString;
            ezStringIterator m_stringView;
        };
    };
}
