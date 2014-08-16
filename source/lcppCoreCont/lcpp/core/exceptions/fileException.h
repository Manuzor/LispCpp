#pragma once

#include "lcpp/core/exceptions/evaluatorException.h"

namespace lcpp
{
    namespace exceptions
    {
        class FileBase :
            public EvaluatorBase
        {
        public:
            FileBase(const String& message) :
                EvaluatorBase(message)
            {
            }
        };

        class InvalidFileMode :
            public FileBase
        {
        public:
            InvalidFileMode(const String& message) :
                FileBase(message)
            {
            }
        };
    }
}
