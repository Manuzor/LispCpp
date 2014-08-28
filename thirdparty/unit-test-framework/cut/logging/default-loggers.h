#pragma once
#include "cut/logging/mode.h"
#include "cut/string-ref.h"

namespace cut
{
	class ILogManager;

	namespace loggers
	{
		struct StdOutWriter
		{
			StdOutWriter(ILogManager& logManager);

			void logMessageHandler(LogMode mode, StringRef formattedMessage);
		};

		struct FileWriter
		{
			FileWriter(ILogManager& logManager, StringRef fileName);
			~FileWriter();

			void logMessageHandler(LogMode mode, StringRef formattedMessage);

		private:
			std::ofstream m_file;
		};

		struct VisualStudioWriter
		{
			VisualStudioWriter(ILogManager& logManager);

			void logMessageHandler(LogMode mode, StringRef formattedMessage);
		};
	}
}

#include "cut/logging/default-loggers.inl"
