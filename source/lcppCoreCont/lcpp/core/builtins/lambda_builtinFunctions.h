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
        }
    }
}
