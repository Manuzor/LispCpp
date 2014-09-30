#pragma once

namespace lcpp
{
    class LispObject;

    namespace cont
    {
        typedef StackPtr<LispObject>(*Function_t)(StackPtr<LispObject>);

        typedef std::size_t UserData_t;
        EZ_CHECK_AT_COMPILETIME(sizeof(UserData_t) >= sizeof(void*));
    }
}
