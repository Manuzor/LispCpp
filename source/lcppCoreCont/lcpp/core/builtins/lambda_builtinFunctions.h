#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace lambda
    {
        namespace builtin
        {
            LCPP_API_CORE_CONT Ptr<LispObject> add(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> subtract(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> multiply(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> divide(Ptr<LispObject> pCont);

            namespace detail
            {

                Ptr<LispObject> add_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> subtract_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> multiply_helper(Ptr<LispObject> pCont);
                Ptr<LispObject> divide_helper(Ptr<LispObject> pCont);
            }
        }
    }
}
