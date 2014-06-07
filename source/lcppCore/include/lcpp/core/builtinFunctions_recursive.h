#pragma once

namespace lcpp
{
    class Environment;
    class IEvaluator;
    class SchemeObject;
    class SchemeCons;
}

namespace lcpp
{
    // Syntax handlers
    namespace syntax
    {
        Ptr<SchemeObject> define(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> lambda(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> if_(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
    }

    // Built-in functions
    namespace builtin
    {
        Ptr<SchemeObject> exit(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> dump(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);

        // REPL
        //////////////////////////////////////////////////////////////////////////
        Ptr<SchemeObject> read(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> eval(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> print(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);

        // File handling
        //////////////////////////////////////////////////////////////////////////
        Ptr<SchemeObject> fileOpen(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> fileIsOpen(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> fileClose(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
        Ptr<SchemeObject> fileReadString(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);

        Ptr<SchemeObject> add(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
    }
}
