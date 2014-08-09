#pragma once

namespace lcpp
{
    namespace exceptions
    {
        class Arithmetic :
            public ExceptionBase
        {
        public:
            Arithmetic(const String& message) :
                ExceptionBase(message)
            {
            }
        };

        class DivisionByZero :
            public Arithmetic
        {
        public:
            DivisionByZero(const String& message = "Division by zero.") :
                Arithmetic(message)
            {
            }
        };
    }
}
