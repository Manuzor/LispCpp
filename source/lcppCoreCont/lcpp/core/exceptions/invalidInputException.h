#pragma once
#include "lcpp/foundation/exceptionBase.h"

namespace lcpp
{
    namespace exceptions
    {
        class InvalidInput :
            public ExceptionBase
        {
        public:
            InvalidInput(const String& message) :
                ExceptionBase(message)
            {
            }
        };
    }
}
