#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace syntax
    {
        namespace builtin
        {
            LCPP_API_CORE_CONT Ptr<LispObject> if_(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> define(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> set(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> begin(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> lambda(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> quote(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> and(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> or(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> time(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> assertion(StackPtr<LispObject> pCont);

            namespace detail
            {
                Ptr<LispObject> if_helper(StackPtr<LispObject> pCont);
                Ptr<LispObject> define_addBinding(StackPtr<LispObject> pCont);
                Ptr<LispObject> begin_helper(StackPtr<LispObject> pCont);
                Ptr<LispObject> lambda_helper(StackPtr<LispObject> pCont);
                Ptr<LispObject> and_helper(StackPtr<LispObject> pCont);
                Ptr<LispObject> or_helper(StackPtr<LispObject> pCont);
                Ptr<LispObject> time_finalize(StackPtr<LispObject> pCont);
                Ptr<LispObject> assertion_finalize(StackPtr<LispObject> pCont);
            }
        }
    }
}
