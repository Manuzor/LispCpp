#pragma once
#include "lcpp/exceptions/exceptions.h"

namespace lcpp { namespace exceptions {

    class InvalidEvalInput :
        public ExceptionBase
    {
    public:
        inline InvalidEvalInput(const char* message = nullptr, const char* file = nullptr, ezUInt32 line = -1) :
            ExceptionBase(message ? message : "Invalid operation!", file, line)
        {}
    };

}}
