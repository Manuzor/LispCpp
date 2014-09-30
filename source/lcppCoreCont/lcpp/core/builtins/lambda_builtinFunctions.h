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
            LCPP_API_CORE_CONT StackPtr<LispObject> add(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> subtract(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> multiply(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> divide(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> modulo(StackPtr<LispObject> pCont);

            // Comparison
            //////////////////////////////////////////////////////////////////////////
            LCPP_API_CORE_CONT StackPtr<LispObject> greaterThan(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> greaterThanOrEqual(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> equal(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> lowerThan(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> lowerThanOrEqual(StackPtr<LispObject> pCont);

            //////////////////////////////////////////////////////////////////////////
            LCPP_API_CORE_CONT StackPtr<LispObject> read(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> eval(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> print(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> exit(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> cons(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> car(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> cdr(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> list(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> eqq(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> recursionLimit(StackPtr<LispObject> pCont);

            namespace detail
            {
                StackPtr<LispObject> add_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> subtract_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> multiply_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> divide_helper(StackPtr<LispObject> pCont);

                StackPtr<LispObject> greaterThan_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> greaterThanOrEqual_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> equal_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> lowerThan_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> lowerThanOrEqual_helper(StackPtr<LispObject> pCont);
            }

            namespace file
            {
                StackPtr<LispObject> open(StackPtr<LispObject> pCont);
                StackPtr<LispObject> isOpen(StackPtr<LispObject> pCont);
                StackPtr<LispObject> close(StackPtr<LispObject> pCont);
                StackPtr<LispObject> readString(StackPtr<LispObject> pCont);
                StackPtr<LispObject> eval(StackPtr<LispObject> pCont);
            }
        }
    }
}
