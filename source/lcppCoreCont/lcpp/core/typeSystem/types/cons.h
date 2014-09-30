#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;
    class Stack;

    namespace cons
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT StackPtr<LispObject> create(StackPtr<LispObject> pCar, StackPtr<LispObject> pCdr);

        LCPP_API_CORE_CONT StackPtr<LispObject> getCar(StackPtr<LispObject> pCons);

        LCPP_API_CORE_CONT StackPtr<LispObject> getCdr(StackPtr<LispObject> pCons);

        LCPP_API_CORE_CONT ezUInt32 pushAll(StackPtr<LispObject> pCons, Ptr<Stack> pStack);
        LCPP_API_CORE_CONT ezUInt32 pushAllReverse(StackPtr<LispObject> pCons, Ptr<Stack> pStack);

        enum { PackEverything = ezUInt32(-1) };

        /// \brief Packs the value-range in the given \a pStack into a cons-list and returns that.
        LCPP_API_CORE_CONT StackPtr<LispObject> pack(Ptr<Stack> pStack, ezInt32 relativeIndexFrom, ezUInt32 maxAmount = PackEverything);

        LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pObject);
    }

    LCPP_API_CORE_CONT bool isCons(StackPtr<LispObject> pObject);
}
