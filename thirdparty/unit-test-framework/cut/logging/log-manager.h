#pragma once

#include "cut/logging/mode.h"
#include "cut/string-ref.h"

namespace cut
{
	struct LogBlockAction
	{
		enum Enum
		{
			Begin,
			End,
		};
	};

	struct LogBlockInfo
	{
		LogBlockAction::Enum action;
		std::size_t indentationLevel;
		std::size_t indentationWidthPerLevel;
		StringRef name;
	};

	typedef std::function<void(const LogBlockInfo&)> BlockListenerFunction_t;

	struct LoggerInfo
	{
		LogMode logMode;
		StringRef message;
		std::size_t indentationLevel;
		std::size_t indentationWidthPerLevel;
	};

	typedef std::function<void(const LoggerInfo&)> LoggerFunction_t;

	class ILogManager
	{
		friend struct LogBlock;
	public:
		CUT_API static ILogManager& instance();

		virtual ~ILogManager() = 0 {}

		/// \remark If you need to format your message, use the function cut::format
		virtual void logMessage(LogMode mode, StringRef formattedMessage) = 0;

		virtual void addLoggerFunction(LoggerFunction_t func) = 0;
		virtual void addBlockListener(BlockListenerFunction_t func) = 0;

		/// \brief Notifies all block listeners before the block level is increased.
		virtual void blockBegin(StringRef blockName) = 0;

		/// \brief Notifies all block listeners after the block level has been decreased.
		virtual void blockEnd(StringRef blockName) = 0;

		virtual std::size_t getBlockIndentation() const = 0;
		virtual void setBlockIndentation(std::size_t value) = 0;

	protected:
		static ILogManager* s_pInstance;
	};
}
