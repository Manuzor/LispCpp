#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    namespace cons
    {

        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::Cons, "cons");

            return meta;
        }

        Ptr<LispObject> create(Ptr<LispObject> pCar, Ptr<LispObject> pCdr)
        {
            auto pInstance = LispObject::create<Data>(metaInfo());
            auto& data = pInstance->m_cons;

            new (data.m_pCar) Ptr<LispObject>(pCar);
            new (data.m_pCdr) Ptr<LispObject>(pCdr);

            return pInstance;
        }


        Ptr<LispObject> getCar(Ptr<LispObject> pCons)
        {
            typeCheck(pCons, Type::Cons);

            return pCons->m_cons.getCar();
        }

        Ptr<LispObject> getCdr(Ptr<LispObject> pCons)
        {
            typeCheck(pCons, Type::Cons);

            return pCons->m_cons.getCdr();
        }

        ezUInt32 pushAll(Ptr<LispObject> pCons, Ptr<Stack> pStack)
        {
            auto count = ezUInt32(0);

            while(isCons(pCons))
            {
                pStack->push(getCar(pCons));
                pCons = getCdr(pCons);
                ++count;
            }

            return count;
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            LCPP_NOT_IMPLEMENTED;
        }
    }

    bool isCons(Ptr<LispObject> pObject)
    {
        return pObject->isType(Type::Cons);
    }

}
