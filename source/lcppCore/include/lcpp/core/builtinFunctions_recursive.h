#pragma once

namespace lcpp
{
    class Environment;
    class IEvaluator;
    class SchemeObject;
}

namespace lcpp { namespace builtin {

    // Syntax
    //////////////////////////////////////////////////////////////////////////
    Ptr<SchemeObject> define(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
    Ptr<SchemeObject> lambda(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);

    // Built-in functions
    Ptr<SchemeObject> exit(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
    Ptr<SchemeObject> dump(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);

    Ptr<SchemeObject> add(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);

}} // namespace lcpp::builtin
