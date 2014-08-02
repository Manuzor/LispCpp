#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace syntax
    {
        namespace builtin
        {
            //LCPP_API_CORE_CONT Ptr<LispObject> define(Ptr<LispObject> pCont);

            //LCPP_API_CORE_CONT Ptr<LispObject> lambda(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> quote(Ptr<LispObject> pCont);
}
    }
}
