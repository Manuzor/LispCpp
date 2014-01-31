#pragma once
#include <exception>

namespace lcpp { namespace exceptions {

    class Base : 
        public std::exception
    {
    public:
        inline Base(const char* file, unsigned int line, const char* message){}

        virtual ~Base() = 0 {}
        const char* file() const { return m_file; }
        const unsigned int line() const { return m_line; }
        const char* message() const { return m_message; }

    protected:
        const char* m_file;
        unsigned int m_line;
        const char* m_message;
    private:
    };

    class InvalidSyntax :
        public Base
    {
    public:
    protected:
    private:
    };

}}