#pragma once

namespace lcpp
{
    class LispObject;

    namespace lambda
    {
        namespace builtin
        {
            typedef StackPtr<LispObject>(*Function_t)(StackPtr<LispObject>);
        }
    }
}
