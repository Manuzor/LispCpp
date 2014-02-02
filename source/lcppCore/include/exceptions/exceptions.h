#pragma once

namespace lcpp { namespace exceptions {

    class ExceptionBase : 
        public std::exception
    {
    public:
        inline ExceptionBase(const char* message = nullptr, const char* file = nullptr, ezInt32 line = -1) :
            m_message(message),
            m_file(file),
            m_line(line)
        {}

        virtual ~ExceptionBase() = 0 {}
        const char* file() const { return m_file; }
        const ezInt32 line() const { return m_line; }
        const char* message() const { return m_message; }

    protected:
        const char* m_file;
        ezInt32 m_line;
        const char* m_message;
    private:
    };

    class NotImplemented :
        public ExceptionBase
    {
    public:
        inline NotImplemented(const char* message = nullptr, const char* file = nullptr, ezInt32 line = -1) :
            ExceptionBase(message ? message : "Not implemented!", file, line)
        {}

        virtual ~NotImplemented() {}
    };

    class InvalidSyntax :
        public ExceptionBase
    {
    public:
        inline InvalidSyntax(const char* message = nullptr, const char* file = nullptr, ezInt32 line = -1) :
            ExceptionBase(message ? message : "Invalid syntax!", file, line)
        {}

        virtual ~InvalidSyntax() {}
    };



    class InvalidOperation :
        public ExceptionBase
    {
    public:
        inline InvalidOperation(const char* message = nullptr, const char* file = nullptr, ezInt32 line = -1) :
            ExceptionBase(message ? message : "Invalid operation!", file, line)
        {}

        virtual ~InvalidOperation() {}
    };

}}