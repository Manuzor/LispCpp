#pragma once

namespace lcpp
{
    class LispRuntime;
    class Environment;

    class LispObject;
}

namespace lcpp
{
    namespace builtin
    {
        Ptr<LispObject> exit(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> dump(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        // REPL
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> read(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> eval(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> print(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        // File handling
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> fileOpen(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> fileIsOpen(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> fileClose(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> fileReadString(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        // Math
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> add(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> sub(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> mul(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> modulo(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> div(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        
        // Comparison
        Ptr<LispObject> equals(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> greaterThan(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> greaterOrEqual(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> lowerThan(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        //Ptr<LispObject> lowerOrEqual(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        
        Ptr<LispObject> objectEquals(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        // Other
        //////////////////////////////////////////////////////////////////////////
        Ptr<LispObject> setRecursionLimit(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> getRecursionLimit(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);

        Ptr<LispObject> cons(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> car(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
        Ptr<LispObject> cdr(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs);
    }
}
