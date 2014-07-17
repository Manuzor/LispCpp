#pragma once

namespace lcpp
{
    class LispRuntime;
    class Environment;

    class LispObject;
}

namespace lcpp
{
    namespace builtIn
    {
        Ptr<LispObject> exit(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> dump(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        // REPL
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> read(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> eval(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> print(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        // File handling
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> fileOpen(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> fileIsOpen(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> fileClose(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> fileReadString(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        // Math
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> add(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> sub(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> mul(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> modulo(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> div(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        
        // Comparison
        Ptr<LispObject> equals(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> greaterThan(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> greaterOrEqual(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> lowerThan(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> lowerOrEqual(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        
        Ptr<LispObject> objectEquals(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        // Other
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> setRecursionLimit(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> getRecursionLimit(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        Ptr<LispObject> cons(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> car(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> cdr(Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
    }
}
