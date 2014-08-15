#pragma once
#include "lcpp/foundation/exceptionBase.h"

namespace lcpp
{
    namespace exceptions
    {
        class EvaluatorBase :
            public ExceptionBase
        {
        public:

            EvaluatorBase(const String& message) :
                ExceptionBase(message)
            {
            }
        };

        class NoBindingFound :
            public EvaluatorBase
        {
        public:
            NoBindingFound(const String& message) :
                EvaluatorBase(message)
            {
            }
        };
    }
}
