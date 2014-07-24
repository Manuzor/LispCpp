#pragma once
#include "cut/testing/assert-handler.h"
#include "cut/string-ref.h"

namespace cut
{
	class DefaultAssertHandler : public IAssertHandler
	{
	public:
		DefaultAssertHandler();
		virtual ~DefaultAssertHandler();

		virtual void handleFailure(const char* file, std::size_t line, StringRef message) CUT_OVERRIDE;
		virtual void handleSuccess(const char* file, std::size_t line, StringRef message) CUT_OVERRIDE;
	};
}
