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
        static void destroy(CollectableBase* pCollectable)
        {
            auto pTime = static_cast<LispObject*>(pCollectable);
            typeCheck(pTime, Type::Time);

            pTime->getData<Data>().m_time.~ezTime();
        }

        static bool isEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            return getTime(pLhs) == getTime(pRhs);
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Time);
                meta.setPrettyName("time");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::DestructorFunction,
                                              static_cast<DestructorFunction_t>(&destroy)));
                    meta.addProperty(MetaProperty(MetaProperty::Builtin::IsEqualFunction, static_cast<IsEqualFunction_t>(&isEqual)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create()
        {
            LCPP_LogBlock("time::create");

            auto pInstance = object::create<Data>(getMetaInfo());

            return pInstance;
        }

        Ptr<LispObject> create(const ezTime& theTime)
        {
            auto pInstance = create();
            pInstance->getData<Data>().m_time = theTime;
            return pInstance;
        }

        ezTime& getTime(Ptr<LispObject> pTime)
        {
            typeCheck(pTime, Type::Time);
            return pTime->getData<Data>().m_time;
        }

        void setTime(Ptr<LispObject> pTime, const ezTime& theTime)
        {
            typeCheck(pTime, Type::Time);
            pTime->getData<Data>().m_time = theTime;
        }

        void setNow(Ptr<LispObject> pTime)
        {
            typeCheck(pTime, Type::Time);
            pTime->getData<Data>().m_time = ezTime::Now();
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pTime)
        {
            typeCheck(pTime, Type::Time);

            ezStringBuilder stringRepresentation;
            stringRepresentation.Format("%fms", pTime->getData<Data>().m_time.GetMilliseconds());

            return str::create(stringRepresentation.GetData(), stringRepresentation.GetElementCount());
        }


    }
}
