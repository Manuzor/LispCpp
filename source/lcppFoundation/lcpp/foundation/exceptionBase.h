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

            void setLineNumber(size_t lineNumber) { m_lineNumber = lineNumber; }
            size_t getLineNumber() const { return m_lineNumber; }

            void setFileName(const char* szFileName) { m_szFileName = szFileName; }
            const char* getFileName() const { return m_szFileName; }

            void setFunctionName(const char* szFunctionName) { m_szFunctionName = szFunctionName; }
            const char* getFunctionName() const { return m_szFunctionName; }

        private:
            String m_message;
            size_t m_lineNumber;
            const char* m_szFileName;
            const char* m_szFunctionName;
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

#define LCPP_THROW(exceptionType, message) do \
{                                             \
    auto ex = exceptionType(message);         \
    ex.setLineNumber(EZ_SOURCE_LINE);         \
    ex.setFileName(EZ_SOURCE_FILE);           \
    ex.setFunctionName(EZ_SOURCE_FUNCTION);   \
    throw ex;                                 \
} while (false)
