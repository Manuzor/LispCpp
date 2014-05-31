#pragma once

namespace lcpp
{
    class Environment;
    class IEvaluator;
    class SchemeObject;
}

namespace lcpp { namespace builtin {

    Ptr<SchemeObject> define(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);
    Ptr<SchemeObject> exit(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs);

}} // namespace lcpp::builtin
