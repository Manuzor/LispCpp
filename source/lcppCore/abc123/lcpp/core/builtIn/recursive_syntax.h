#pragma once

namespace lcpp
{
    class SchemeRuntime;
    class Environment;

    class SchemeObject;
}

namespace lcpp
{
    namespace syntax
    {
        Ptr<SchemeObject> define(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> set(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> lambda(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> if_(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> and(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> or(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
    }
}
