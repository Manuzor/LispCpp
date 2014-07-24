#pragma once
#include "cut/string-ref.h"

namespace cut
{
	struct CUT_API Assert
	{
		const char* fileName;
		std::size_t lineNumber;
		
		Assert(const char* fileName, std::size_t lineNumber);

		void isTrue(bool expr, StringRef message = nullptr);
		void isFalse(bool expr, StringRef message = nullptr);

		template<typename T_Exception>
		void throws(Lambda_t lambda, StringRef message = nullptr);
		void throwsNothing(Lambda_t lambda, StringRef message = nullptr);

		void fail(StringRef message = nullptr);
		void succeed(StringRef message = nullptr);
	};
}

#include "cut/testing/assert.inl"

#define CUT_ASSERT ::cut::Assert(CUT_SOURCE_FILE, CUT_SOURCE_LINE)
