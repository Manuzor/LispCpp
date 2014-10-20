#pragma once

namespace lcpp
{
    class LispObject;

    namespace lambda
    {
        namespace builtin
        {
            typedef Ptr<LispObject>(*Function_t)(StackPtr<LispObject>);
        }
    }
}
