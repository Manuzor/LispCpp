#pragma once

namespace lcpp
{
    class LispRuntime;
    class Environment;

    class LispObject;
}

namespace lcpp
{
    namespace syntax
    {
        Ptr<LispObject> define(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> set(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> lambda(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> if_(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> and(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> or(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
    }
}
