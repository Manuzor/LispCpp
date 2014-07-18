#pragma once

namespace lcpp
{
    class LispRuntime;
    class LispEnvironment;

    class LispObject;
}

namespace lcpp
{
    namespace syntax
    {
        Ptr<LispObject> define(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> set(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> lambda(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> if_(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> and(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> or(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
    }
}
