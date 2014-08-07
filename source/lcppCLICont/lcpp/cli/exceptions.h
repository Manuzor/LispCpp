#pragma once

namespace lcpp { namespace exceptions {

    class InitializationFailed :
        public ExceptionBase
    {
    public:
        inline InitializationFailed(const char* message = nullptr, const char* file = nullptr, ezUInt32 line = -1) :
        ExceptionBase(message ? message : "Not implemented!")
        {}
    };

}} // namespace lcpp::exceptions
