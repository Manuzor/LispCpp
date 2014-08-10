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

            // Comparison
            //////////////////////////////////////////////////////////////////////////
            LCPP_API_CORE_CONT Ptr<LispObject> greaterThan(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> greaterThanOrEqual(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> equal(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> lowerThan(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> lowerThanOrEqual(Ptr<LispObject> pCont);

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
        }
    }
}
