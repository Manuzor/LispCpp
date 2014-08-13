#pragma once
#include "lcpp/foundation/exceptionBase.h"

namespace lcpp
{
    namespace exceptions
    {
        class Exit :
            public ExceptionBase
        {
        public:
            Exit(ezInt32 exitCode, const String& message) :
                ExceptionBase(message),
                m_exitCode(exitCode)
            {
            }

        public:

            ezInt32 getExitCode() const { return m_exitCode; }

        private:

            ezInt32 m_exitCode;
        };
    }
}
