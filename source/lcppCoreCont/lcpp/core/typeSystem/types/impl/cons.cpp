#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectUtils.h"

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

        static void toStringHelper(Ptr<LispObject> pCons, ezStringBuilder& builder)
        {
            auto pCar = getCar(pCons);

            if (isCons(pCar))
            {
                builder.Append('(');
                toStringHelper(pCar, builder);
                builder.Append(')');
            }
            else
            {
                builder.Append(str::getValue(lcpp::toString(pCar)).GetData());
            }

            auto pCdr = getCdr(pCons);

            if (isNil(pCdr))
            {
                return;
            }
            
            builder.Append(' ');

            if (isCons(pCdr))
            {
                toStringHelper(pCdr, builder);
            }
            else
            {
                builder.AppendFormat(". %s", str::getValue(lcpp::toString(pCdr)).GetData());
            }
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Cons);
            
            auto builder = ezStringBuilder();
            builder.Append('(');
            toStringHelper(pObject, builder);
            builder.Append(')');

            return str::create(builder);
        }
    }

    bool isCons(Ptr<LispObject> pObject)
    {
        return pObject->isType(Type::Cons);
    }

}
