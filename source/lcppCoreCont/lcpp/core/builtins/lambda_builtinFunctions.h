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
            LCPP_API_CORE_CONT Ptr<LispObject> add(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> subtract(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> multiply(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> divide(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> modulo(Ptr<LispObject> pCont);

            // Comparison
            //////////////////////////////////////////////////////////////////////////
            LCPP_API_CORE_CONT Ptr<LispObject> greaterThan(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> greaterThanOrEqual(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> equal(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> lowerThan(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> lowerThanOrEqual(Ptr<LispObject> pCont);

            //////////////////////////////////////////////////////////////////////////
            LCPP_API_CORE_CONT Ptr<LispObject> read(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> eval(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> print(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> exit(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> cons(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> car(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> cdr(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> list(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> eqq(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> recursionLimit(Ptr<LispObject> pCont);

            namespace detail
            {
                Ptr<LispObject> add_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> subtract_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> multiply_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> divide_helper(Ptr<LispObject> pCont);

                Ptr<LispObject> greaterThan_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> greaterThanOrEqual_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> equal_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> lowerThan_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> lowerThanOrEqual_helper(Ptr<LispObject> pCont);
            }

            namespace file
            {
                Ptr<LispObject> open(Ptr<LispObject> pCont);
                Ptr<LispObject> isOpen(Ptr<LispObject> pCont);
                Ptr<LispObject> close(Ptr<LispObject> pCont);
                Ptr<LispObject> readString(Ptr<LispObject> pCont);
                Ptr<LispObject> eval(Ptr<LispObject> pCont);
            }
        }
    }
}
