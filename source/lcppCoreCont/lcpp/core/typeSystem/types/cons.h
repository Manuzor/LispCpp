#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;
    class Stack;

    namespace cons
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pCar, Ptr<LispObject> pCdr);

        LCPP_API_CORE_CONT Ptr<LispObject> getCar(Ptr<LispObject> pCons);

        LCPP_API_CORE_CONT Ptr<LispObject> getCdr(Ptr<LispObject> pCons);

        LCPP_API_CORE_CONT ezUInt32 pushAll(Ptr<LispObject> pCons, Ptr<Stack> pStack);
        LCPP_API_CORE_CONT ezUInt32 pushAllReverse(Ptr<LispObject> pCons, Ptr<Stack> pStack);

        enum { PackEverything = ezUInt32(-1) };

        /// \brief Packs the value-range in the given \a pStack into a cons-list and returns that.
        LCPP_API_CORE_CONT Ptr<LispObject> pack(Ptr<Stack> pStack, ezInt32 relativeIndexFrom, ezUInt32 maxAmount = PackEverything);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);
    }

    LCPP_API_CORE_CONT bool isCons(Ptr<LispObject> pObject);
}
