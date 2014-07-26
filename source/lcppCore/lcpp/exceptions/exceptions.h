#pragma once

namespace lcpp { namespace exceptions {

    class InvalidSyntax :
        public ExceptionBase
    {
    public:
        inline InvalidSyntax(const char* message = nullptr) :
            ExceptionBase(message ? message : "Invalid syntax!")
        {}
    };

    class InvalidInput :
        public ExceptionBase
    {
    public:
        inline InvalidInput(const char* message = nullptr) :
            ExceptionBase(message ? message : "Invalid syntax!")
        {}
    };

    class NoBindingFound :
        public InvalidInput
    {
    public:
        inline NoBindingFound(const char* message) :
            InvalidInput(message)
        {
        }
    };

    class InvalidOperation :
        public ExceptionBase
    {
    public:
        inline InvalidOperation(const char* message = nullptr) :
            ExceptionBase(message ? message : "Invalid operation!")
        {}
    };

    class Exit :
        public ExceptionBase
    {
    public:
        inline Exit(const char* message = nullptr, ezInt32 status = 0) :
            ExceptionBase(message ? message : "Good bye."),
            m_status(status)
        {}

        ezInt32 m_status;
    };

    class Runtime :
        public ExceptionBase
    {
    public:
        inline Runtime(const char* message) :
            ExceptionBase(message)
        {
        }
    };

    class Assertion :
        public ExceptionBase
    {
    public:
        inline Assertion(const char* message) :
            ExceptionBase(message)
        {
        }
    };
    
}}
