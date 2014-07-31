#pragma once
#include "lcpp/foundation/exceptionBase.h"

namespace lcpp
{
    namespace exceptions
    {
        class NoBindingFound :
            public ExceptionBase
        {
        public:
            NoBindingFound(const String& message) :
                ExceptionBase(message)
            {
            }
        };
    }
}
