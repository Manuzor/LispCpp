#pragma once
#include "cut/logging/log-manager.h"
#include "cut/logging/mode.h"

namespace cut
{
	class DefaultLogManager : public ILogManager
	{
	public:
		DefaultLogManager();
		virtual ~DefaultLogManager();

		virtual void logMessage(LogMode mode, StringRef formattedMessage) CUT_OVERRIDE;

		virtual void addLoggerFunction(LoggerFunction_t func) CUT_OVERRIDE;
		virtual void addBlockListener(BlockListenerFunction_t func) CUT_OVERRIDE;

		virtual std::size_t getBlockIndentation() const CUT_OVERRIDE;
		virtual void setBlockIndentation(std::size_t value) CUT_OVERRIDE;

		virtual void blockBegin(StringRef blockName) CUT_OVERRIDE;
		virtual void blockEnd(StringRef blockName) CUT_OVERRIDE;

	private:

		std::size_t m_blockLevel;
		std::size_t m_blockIndentation;
		std::vector<LoggerFunction_t> m_loggers;
		std::vector<BlockListenerFunction_t> m_blockListeners;
	};
}
