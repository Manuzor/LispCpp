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
        Ptr<LispObject> define(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> set(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> lambda(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> if_(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> and(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> or(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
    }
}
