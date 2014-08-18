#pragma once

#include "lcpp/core/exceptions/evaluatorException.h"

namespace lcpp
{
    namespace exceptions
    {
        class AssertionBase :
            public EvaluatorBase
        {
        public:
            AssertionBase(const String& message) :
                EvaluatorBase(message)
            {
            }
        };

        class UserAssertionFailed :
            public AssertionBase
        {
        public:
            UserAssertionFailed(const String& message) :
                AssertionBase(message)
            {
            }
        };
    }
}
