#pragma once

#include "cut/logging/mode.h"
#include "cut/string-ref.h"

namespace cut
{
	struct LogBlock;

	class ILogManager
	{
		friend struct LogBlock;
	public:
		CUT_API static ILogManager& instance();

		virtual ~ILogManager() = 0 {}

		/// \remark If you need to format your message, use cut::format
		virtual void logMessage(LogMode mode, StringRef formattedMessage) = 0;

		virtual void setLogFileName(StringRef fileName) = 0;
		virtual const StringRef getLogFileName() const = 0;

		virtual void blockBegin() = 0;
		virtual void blockEnd() = 0;

		virtual std::size_t getBlockIndentation() const = 0;
		virtual void setBlockIndentation(std::size_t value) = 0;

	protected:
		static ILogManager* s_pInstance;
	};
}
