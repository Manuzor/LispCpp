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

            auto& data = pInstance->getData<Data>();

            new (data.m_time) ezTime();

            return pInstance;
        }

        Ptr<LispObject> create(const ezTime& theTime)
        {
            auto pInstance = create();
            pInstance->getData<Data>().setTime(theTime);
            return pInstance;
        }

        ezTime& getTime(Ptr<LispObject> pTime)
        {
            typeCheck(pTime, Type::Time);
            return pTime->getData<Data>().getTime();
        }

        void setTime(Ptr<LispObject> pTime, const ezTime& theTime)
        {
            typeCheck(pTime, Type::Time);
            pTime->getData<Data>().setTime(theTime);
        }

        void setNow(Ptr<LispObject> pTime)
        {
            typeCheck(pTime, Type::Time);
            pTime->getData<Data>().setTime(ezTime::Now());
        }

        Ptr<LispObject> toString(Ptr<LispObject> pTime)
        {
            typeCheck(pTime, Type::Time);

            auto stringRepresentation = ezStringBuilder();
            stringRepresentation.Format("%fms", pTime->getData<Data>().getTime().GetMilliseconds());

            return str::create(stringRepresentation.GetData());
        }


    }
}
