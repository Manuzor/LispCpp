#include "stdafx.h"
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/consData.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/nil.h"

#include "lcpp/core/preprocessorUtils/disableVerboseLoggingLocally.h"

namespace lcpp
{
    namespace cons
    {
        static void scan(CollectableBase* pCollectable, GarbageCollectionContext* pGC)
        {
            Ptr<LispObject> pObject(static_cast<LispObject*>(pCollectable));
            typeCheck(pObject, Type::Cons);

            {
                auto pCar = pObject->getData<Data>().getCar().get();
                pCar = pGC->addSurvivor(pCar);
            }

            {
                auto pCdr = pObject->getData<Data>().getCar().get();
                pCdr = pGC->addSurvivor(pCdr);
            }
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Cons);
                meta.setPrettyName("cons");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::ScanFunction, static_cast<ScanFunction_t>(&scan)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create(Ptr<LispObject> pCar, Ptr<LispObject> pCdr)
        {
            LCPP_LogBlock("cons::create");

            auto pInstance = object::create<Data>(getMetaInfo());
            auto& data = pInstance->getData<Data>();

            new (data.m_pCar) Ptr<LispObject>(pCar);
            new (data.m_pCdr) Ptr<LispObject>(pCdr);

            return pInstance;
        }


        Ptr<LispObject> getCar(Ptr<LispObject> pCons)
        {
            typeCheck(pCons, Type::Cons);

            return pCons->getData<Data>().getCar();
        }

        Ptr<LispObject> getCdr(Ptr<LispObject> pCons)
        {
            typeCheck(pCons, Type::Cons);

            return pCons->getData<Data>().getCdr();
        }

        ezUInt32 pushAll(Ptr<LispObject> pCons, Ptr<Stack> pStack)
        {
            LCPP_LogBlock("cons::pushAll");
            LCPP_LogVerboseDebugMessage("Cons = %s", str::getValue(object::toString(pCons)).GetData());
            ezUInt32 count(0);

            while(isCons(pCons))
            {
                pStack->push(getCar(pCons));
                pCons = getCdr(pCons);
                ++count;
            }
            LCPP_LogVerboseDebugMessage("%u", count);
            return count;
        }

        ezUInt32 pushAllReverse(Ptr<LispObject> pCons, Ptr<Stack> pStack)
        {
            LCPP_LogBlock("cons::pushAllReverse");
            LCPP_LogVerboseDebugMessage("Cons = %s", str::getValue(object::toString(pCons)).GetData());
            Stack tempStack;

            auto count = pushAll(pCons, &tempStack);

            while(!tempStack.isEmpty())
            {
                auto pObject = tempStack.get(-1);
                tempStack.pop();
                pStack->push(pObject);
            }

            LCPP_LogVerboseDebugMessage("%u", count);
            return count;
        }

        Ptr<LispObject> pack(Ptr<Stack> pStack, ezInt32 relativeIndexFrom, ezUInt32 maxAmount)
        {
            LCPP_LogBlock("cons::pack");

            const auto startIndex = pStack->convertToAbsolute(relativeIndexFrom);
            auto amount = ezMath::Min(pStack->size(), pStack->size() - startIndex, maxAmount);

            auto pCons = LCPP_pNil;
            Stack tempStack;

            for(auto i = ezUInt32(0); i < amount; ++i)
            {
                auto index = ezInt32(relativeIndexFrom + i);
                auto pCar = pStack->get(index);
                tempStack.push(pCar);
            }

            while(!tempStack.isEmpty())
            {
                auto pCar = tempStack.get(-1);
                tempStack.pop();
                pCons = cons::create(pCar, pCons);
            }

            return pCons;
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
                builder.Append(str::getValue(object::toString(pCar)).GetData());
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
                builder.AppendFormat(". %s", str::getValue(object::toString(pCdr)).GetData());
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
        return object::isType(pObject, Type::Cons);
    }

}
