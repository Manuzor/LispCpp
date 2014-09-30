#pragma once

namespace lcpp
{
    class LispObject;

    namespace syntax
    {
        namespace builtin
        {

            typedef StackPtr<LispObject>(*Function_t)(StackPtr<LispObject>);
        }
    }
}
