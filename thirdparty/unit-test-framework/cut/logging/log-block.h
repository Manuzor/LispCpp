#pragma once
#include "cut/string-ref.h"

namespace cut
{
	struct LogBlockImpl;

	struct LogBlock
	{
		LogBlock(StringRef name);
		~LogBlock();

	private:
		std::string m_name;
	};
}

#include "cut/logging/log-block.inl"
