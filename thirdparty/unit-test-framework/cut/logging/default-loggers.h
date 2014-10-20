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

			void logMessageHandler(const LoggerInfo& loggerInfo);
			void blockHandler(const LogBlockInfo& blockinfo);
		};

		struct FileWriter
		{
			FileWriter(ILogManager& logManager, StringRef fileName);
			~FileWriter();

			void logMessageHandler(const LoggerInfo& loggerInfo);
			void blockHandler(const LogBlockInfo& blockinfo);

		private:
			std::ofstream m_file;
		};

		struct VisualStudioWriter
		{
			VisualStudioWriter(ILogManager& logManager);

			void logMessageHandler(const LoggerInfo& loggerInfo);
			void blockHandler(const LogBlockInfo& blockinfo);
		};
	}
}

#include "cut/logging/default-loggers.inl"
