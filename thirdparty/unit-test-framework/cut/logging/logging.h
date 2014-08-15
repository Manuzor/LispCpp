#pragma once

#include "cut/logging/log-manager.h"
#include "cut/logging/log-block.h"
#include "cut/string-ref.h"

namespace cut
{
	inline void log(LogMode mode, StringRef message)
	{
		ILogManager::instance().logMessage(mode, message);
	}

	inline void logMessage(StringRef message)
	{
		log(LogMode::Normal, message);
	}

	inline void logWarning(StringRef message)
	{
		log(LogMode::Warning, message);
	}

	inline void logSuccess(StringRef message)
	{
		log(LogMode::Success, message);
	}

	inline void logFailure(StringRef message)
	{
		log(LogMode::Failure, message);
	}
}
