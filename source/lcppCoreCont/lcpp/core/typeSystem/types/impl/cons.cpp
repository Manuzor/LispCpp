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
#include "lcpp/core/typeSystem/types/continuation.h"

namespace lcpp
{
    namespace cons
    {
        static void scan(CollectableBase* pCollectable, GarbageCollectionContext* pGC)
        {
            Ptr<LispObject> pObject(static_cast<LispObject*>(pCollectable));
            typeCheck(pObject, Type::Cons);

            {
                auto& pCar = pObject->getData<Data>().m_pCar.get();
                pCar = pGC->addSurvivor(pCar);
            }

            {
                auto& pCdr = pObject->getData<Data>().m_pCdr.get();
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

        Ptr<LispObject> create(StackPtr<LispObject> pCar, StackPtr<LispObject> pCdr)
        {
            LCPP_LogBlock("cons::create");

            auto pInstance = object::create<Data>(getMetaInfo());

            LCPP_GC_PreventCollectionInScope;
            auto& data = pInstance->getData<Data>();

            data.m_pCar = pCar.get();
            data.m_pCdr = pCdr.get();

            return pInstance;
        }


        Ptr<LispObject> getCar(Ptr<LispObject> pCons)
        {
            typeCheck(pCons, Type::Cons);

            return pCons->getData<Data>().m_pCar;
        }

        Ptr<LispObject> getCdr(Ptr<LispObject> pCons)
        {
            typeCheck(pCons, Type::Cons);

            return pCons->getData<Data>().m_pCdr;
        }

        ezUInt32 pushAll(Ptr<LispObject> pCons, Ptr<Stack> pStack)
        {
            LCPP_GC_PreventCollectionInScope;

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
            LCPP_GC_PreventCollectionInScope;

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

        Ptr<LispObject> pack(StackPtr<LispObject> pCont, ezInt32 relativeIndexFrom, ezUInt32 maxAmount)
        {
            LCPP_LogBlock("cons::pack");

            /// Calculating the indices:
            ///       [0 1 2 3 4 5]
            /// low  --^
            /// high ------------^
            /// amount = 6
            /// => high = low + amount - 1; => high = 0 + 6 - 1 = 5

            const ezInt32 lowIndex = cont::getStack(pCont)->convertToAbsolute(relativeIndexFrom);
            EZ_ASSERT(lowIndex >= 0, "");
            ezInt32 amount = ezMath::Min(cont::getStack(pCont)->size() - lowIndex, maxAmount);
            const ezInt32 highIndex = lowIndex + amount - 1;
            EZ_ASSERT(highIndex >= lowIndex, "");

            StackPtr<LispObject> pCons = LCPP_pNil;

            for (auto index = highIndex;
                 amount > 0;
                 --amount, --index)
            {
                pCons = cons::create(cont::getStack(pCont)->get(index), pCons);
            }

            return pCons;
        }

        static void toStringHelper(StackPtr<LispObject> pCons, ezStringBuilder& builder)
        {
            StackPtr<LispObject> pCar = getCar(pCons);

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

            StackPtr<LispObject> pCdr = getCdr(pCons);

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

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
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
