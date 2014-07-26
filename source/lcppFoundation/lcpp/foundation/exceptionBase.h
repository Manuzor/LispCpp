#pragma once
#include <exception>

namespace lcpp
{
    namespace exceptions
    {
        class ExceptionBase :
            public std::exception
        {
        public:

            ExceptionBase(const String& message) : m_message(message) {}

            virtual const char* what() const override { return m_message.GetData(); }

        private:
            String m_message;
        };
        
        class NotImplemented :
            public ExceptionBase
        {
        public:

            NotImplemented() : ExceptionBase("Not implemented.") {}

            NotImplemented(const String& message) : ExceptionBase(message) {}
        };
    }
}
