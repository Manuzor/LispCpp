#pragma once

namespace lcpp
{
    class LispObject;

    namespace syntax
    {
        namespace builtin
        {

            typedef Ptr<LispObject>(*Function_t)(Ptr<LispObject>);
        }
    }
}
