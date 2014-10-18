#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace lambda
    {
        namespace builtin
        {
            // Arithmetic
            //////////////////////////////////////////////////////////////////////////
            LCPP_API_CORE_CONT Ptr<LispObject> add(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> subtract(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> multiply(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> divide(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> modulo(StackPtr<LispObject> pCont);

            // Comparison
            //////////////////////////////////////////////////////////////////////////
            LCPP_API_CORE_CONT Ptr<LispObject> greaterThan(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> greaterThanOrEqual(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> equal(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> lowerThan(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> lowerThanOrEqual(StackPtr<LispObject> pCont);

            //////////////////////////////////////////////////////////////////////////
            LCPP_API_CORE_CONT Ptr<LispObject> read(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> eval(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> print(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> exit(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> cons(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> car(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> cdr(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> list(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> eqq(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> recursionLimit(StackPtr<LispObject> pCont);

            namespace detail
            {
                Ptr<LispObject> greaterThan_helper(StackPtr<LispObject> pCont);
                Ptr<LispObject> greaterThanOrEqual_helper(StackPtr<LispObject> pCont);
                Ptr<LispObject> equal_helper(StackPtr<LispObject> pCont);
                Ptr<LispObject> lowerThan_helper(StackPtr<LispObject> pCont);
                Ptr<LispObject> lowerThanOrEqual_helper(StackPtr<LispObject> pCont);
            }

            namespace file
            {
                Ptr<LispObject> open(StackPtr<LispObject> pCont);
                Ptr<LispObject> isOpen(StackPtr<LispObject> pCont);
                Ptr<LispObject> close(StackPtr<LispObject> pCont);
                Ptr<LispObject> readString(StackPtr<LispObject> pCont);
                Ptr<LispObject> eval(StackPtr<LispObject> pCont);
            }
        }
    }
}
