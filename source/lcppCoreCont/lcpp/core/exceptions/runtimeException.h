#pragma once

namespace lcpp
{
    namespace exceptions
    {
        class Runtime :
            public ExceptionBase
        {
        public:
            Runtime(const String& message) :
                ExceptionBase(message)
            {
            }
        };
    }
}
