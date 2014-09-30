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
        static void scan(Ptr<CollectableBase> pCollectable, GarbageCollector::PatchablePointerArray& pointersToPatch)
        {
            auto pObject = pCollectable.cast<LispObject>();
            typeCheck(pObject, Type::Cons);

            auto ppCar = &reinterpret_cast<Ptr<CollectableBase>&>(pObject->getData<Data>().m_pCar);
            pointersToPatch.PushBack(ppCar);
            
            auto ppCdr = &reinterpret_cast<Ptr<CollectableBase>&>(pObject->getData<Data>().m_pCdr);
            pointersToPatch.PushBack(ppCdr);
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Cons);
                meta.setPrettyName("cons");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::ScanFunction, &scan));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        StackPtr<LispObject> create(StackPtr<LispObject> pCar, StackPtr<LispObject> pCdr)
        {
            LCPP_LogBlock("cons::create");

            auto pInstance = object::create<Data>(getMetaInfo());
            auto& data = pInstance->getData<Data>();

            new (data.m_pCar) Ptr<LispObject>(pCar);
            new (data.m_pCdr) Ptr<LispObject>(pCdr);

            return pInstance;
        }


        StackPtr<LispObject> getCar(StackPtr<LispObject> pCons)
        {
            typeCheck(pCons, Type::Cons);

            return pCons->getData<Data>().getCar();
        }

        StackPtr<LispObject> getCdr(StackPtr<LispObject> pCons)
        {
            typeCheck(pCons, Type::Cons);

            return pCons->getData<Data>().getCdr();
        }

        ezUInt32 pushAll(StackPtr<LispObject> pCons, Ptr<Stack> pStack)
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

        ezUInt32 pushAllReverse(StackPtr<LispObject> pCons, Ptr<Stack> pStack)
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

        StackPtr<LispObject> pack(Ptr<Stack> pStack, ezInt32 relativeIndexFrom, ezUInt32 maxAmount)
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

        static void toStringHelper(StackPtr<LispObject> pCons, ezStringBuilder& builder)
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

        StackPtr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Cons);
            
            auto builder = ezStringBuilder();
            builder.Append('(');
            toStringHelper(pObject, builder);
            builder.Append(')');

            return str::create(builder);
        }

    }

    bool isCons(StackPtr<LispObject> pObject)
    {
        return object::isType(pObject, Type::Cons);
    }

}
