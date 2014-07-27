#pragma once
#include "lcpp/foundation/exceptionBase.h"

namespace lcpp
{
    namespace exceptions
    {
        class TypeCheckFailed :
            public ExceptionBase
        {
        public:
            TypeCheckFailed(const String& message) :
                ExceptionBase(message)
            {
            }
        };
    }
}
