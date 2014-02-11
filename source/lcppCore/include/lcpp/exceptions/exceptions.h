#pragma once

namespace lcpp { namespace exceptions {

    class ExceptionBase : 
        public std::exception
    {
    public:
        inline ExceptionBase(const char* message = nullptr, const char* file = nullptr, ezUInt32 line = 0) :
            m_message(message),
            m_file(file),
            m_line(line)
        {}

        inline virtual ~ExceptionBase() = 0 {}
        inline const char* file() const { return m_file; }
        inline const ezUInt32 line() const { return m_line; }
        inline const char* message() const { return m_message; }

    protected:
        const char* m_file;
        ezUInt32 m_line;
        const char* m_message;
    private:
    };

    class NotImplemented :
        public ExceptionBase
    {
    public:
        inline NotImplemented(const char* message = nullptr, const char* file = nullptr, ezUInt32 line = -1) :
            ExceptionBase(message ? message : "Not implemented!", file, line)
        {}

        inline virtual ~NotImplemented() {}
    };

    class InvalidSyntax :
        public ExceptionBase
    {
    public:
        inline InvalidSyntax(const char* message = nullptr, const char* file = nullptr, ezUInt32 line = -1) :
            ExceptionBase(message ? message : "Invalid syntax!", file, line)
        {}

        inline virtual ~InvalidSyntax() {}
    };

    class InvalidOperation :
        public ExceptionBase
    {
    public:
        inline InvalidOperation(const char* message = nullptr, const char* file = nullptr, ezUInt32 line = -1) :
            ExceptionBase(message ? message : "Invalid operation!", file, line)
        {}

        inline virtual ~InvalidOperation() {}
    };

}}