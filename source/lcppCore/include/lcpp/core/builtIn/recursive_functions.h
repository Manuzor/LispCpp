#pragma once

namespace lcpp
{
    class SchemeRuntime;
    class Environment;

    class SchemeObject;
}

namespace lcpp
{
    namespace builtin
    {
        Ptr<SchemeObject> exit(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> dump(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);

        // REPL
        //////////////////////////////////////////////////////////////////////////
        Ptr<SchemeObject> read(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> eval(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> print(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);

        // File handling
        //////////////////////////////////////////////////////////////////////////
        Ptr<SchemeObject> fileOpen(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> fileIsOpen(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> fileClose(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> fileReadString(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);

        // Math
        //////////////////////////////////////////////////////////////////////////
        Ptr<SchemeObject> add(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);

        // Other
        //////////////////////////////////////////////////////////////////////////
        Ptr<SchemeObject> setRecursionLimit(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> getRecursionLimit(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs);
    }
}
