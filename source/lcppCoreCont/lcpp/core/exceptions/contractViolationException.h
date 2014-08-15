#pragma once
#include "lcpp/foundation/exceptionBase.h"

namespace lcpp
{
    namespace exceptions
    {
        class ContractViolation :
            public ExceptionBase
        {
        public:
            ContractViolation(const String& message) :
                ExceptionBase(message)
            {
            }
        };
    }
}
