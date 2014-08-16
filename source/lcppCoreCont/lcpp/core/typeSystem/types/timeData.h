#pragma once

namespace lcpp
{
    namespace time
    {
        class Data
        {
        public:

            ezTime& getTime();
            const ezTime& getTime() const;
            void setTime(const ezTime& value);

        public:

            LCPP_DeclareRawDataMember(ezTime, m_time);
        };
    }
}

#include "lcpp/core/typeSystem/types/impl/timeData.inl"
