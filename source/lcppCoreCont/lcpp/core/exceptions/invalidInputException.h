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

        class ArgumentCount :
            public InvalidInput
        {
        public:
            ArgumentCount(const String& message) :
                InvalidInput(message)
            {
            }
        };

        class ArgumentType :
            public InvalidInput
        {
        public:
            ArgumentType(const String& message) :
                InvalidInput(message)
            {
            }
        };
    }
}
