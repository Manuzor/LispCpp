#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace syntax
    {
        namespace builtin
        {
            LCPP_API_CORE_CONT Ptr<LispObject> define(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> begin(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> lambda(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> quote(Ptr<LispObject> pCont);

            namespace detail
            {
                Ptr<LispObject> define_2(Ptr<LispObject> pCont);
                Ptr<LispObject> begin_helper(Ptr<LispObject> pCont);
            }
        }
    }
}
