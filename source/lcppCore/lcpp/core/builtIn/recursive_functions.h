#pragma once

namespace lcpp
{
    class LispRuntime;
    class LispEnvironment;

    class LispObject;
}

namespace lcpp
{
    namespace builtIn
    {
        Ptr<LispObject> exit(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> dump(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);

        // REPL
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> read(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> eval(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> print(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);

        // File handling
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> fileOpen(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> fileIsOpen(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> fileClose(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> fileReadString(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);

        // Math
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> add(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> sub(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> mul(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> modulo(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> div(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        
        // Comparison
        Ptr<LispObject> equals(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> greaterThan(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> greaterOrEqual(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> lowerThan(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> lowerOrEqual(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        
        Ptr<LispObject> objectEquals(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);

        // Other
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> setRecursionLimit(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> getRecursionLimit(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);

        Ptr<LispObject> cons(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> car(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> cdr(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs);
    }
}
