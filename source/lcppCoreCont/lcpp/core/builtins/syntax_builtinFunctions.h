#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace syntax
    {
        namespace builtin
        {
            LCPP_API_CORE_CONT StackPtr<LispObject> if_(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> define(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> set(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> begin(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> lambda(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> quote(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> and(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> or(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> time(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> assertion(StackPtr<LispObject> pCont);

            namespace detail
            {
                StackPtr<LispObject> if_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> define_addBinding(StackPtr<LispObject> pCont);
                StackPtr<LispObject> begin_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> lambda_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> and_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> or_helper(StackPtr<LispObject> pCont);
                StackPtr<LispObject> time_finalize(StackPtr<LispObject> pCont);
                StackPtr<LispObject> assertion_finalize(StackPtr<LispObject> pCont);
            }
        }
    }
}
