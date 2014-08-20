#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"

#include "lcpp/core/typeSystem/types/time.h"
#include "lcpp/core/typeSystem/types/timeData.h"
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    namespace time
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::Time, "time");
            return meta;
        }

        Ptr<LispObject> create()
        {
            auto pInstance = object::create<Data>(metaInfo());

            auto& data = pInstance->m_time;

            new (data.m_time) ezTime();

            return pInstance;
        }

        Ptr<LispObject> create(const ezTime& theTime)
        {
            auto pInstance = create();
            pInstance->m_time.setTime(theTime);
            return pInstance;
        }

        ezTime& getTime(Ptr<LispObject> pTime)
        {
            typeCheck(pTime, Type::Time);
            return pTime->m_time.getTime();
        }

        void setTime(Ptr<LispObject> pTime, const ezTime& theTime)
        {
            typeCheck(pTime, Type::Time);
            pTime->m_time.setTime(theTime);
        }

        void setNow(Ptr<LispObject> pTime)
        {
            typeCheck(pTime, Type::Time);
            pTime->m_time.setTime(ezTime::Now());
        }

        Ptr<LispObject> toString(Ptr<LispObject> pTime)
        {
            typeCheck(pTime, Type::Time);

            auto stringRepresentation = ezStringBuilder();
            stringRepresentation.Format("%fms", pTime->m_time.getTime().GetMilliseconds());

            return str::create(stringRepresentation.GetData());
        }


    }
}
