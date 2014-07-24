#pragma once
#include "cut/string-ref.h"

namespace cut
{
	class IAssertHandler
	{
	public:
		CUT_API static IAssertHandler& instance();

		virtual ~IAssertHandler() {}

		virtual void handleFailure(const char* file, std::size_t line, StringRef message) = 0;
		virtual void handleSuccess(const char* file, std::size_t line, StringRef message) = 0;

	protected:
		/// Overwrite this in your custom assert handler
		static IAssertHandler* s_pInstance;
	};
}
