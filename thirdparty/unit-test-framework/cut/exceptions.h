#pragma once
#include <exception>
#include "cut/string-ref.h"

namespace cut
{
	namespace exceptions
	{
		class ExpectionBase : public std::exception
		{
		public:
			ExpectionBase(const char* file, std::size_t line, StringRef message);
			virtual ~ExpectionBase();
			
			const char* file() const { return m_file; }
			const std::size_t line() const { return m_line; }

			const StringRef message() const { return m_message; }
			StringRef       message()       { return m_message; }

		private:
			const char* m_file;
			std::size_t m_line;
			std::string m_message;
		};

		class UnitTestFailure : public ExpectionBase
		{
		public:
			UnitTestFailure(const char* file, std::size_t line, StringRef message);
			virtual ~UnitTestFailure();
		};

		class UnitTestSuccess : public ExpectionBase
		{
		public:
			UnitTestSuccess();
			UnitTestSuccess(const char* file, std::size_t line, StringRef message);
			virtual ~UnitTestSuccess();
		};
	}
}
