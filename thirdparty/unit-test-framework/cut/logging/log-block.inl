
#include "cut/string-format.h"

inline
cut::LogBlock::LogBlock(StringRef name) :
	m_begin(),
	m_end()
{
	m_begin = format(">> %s", name.cString());
	m_end = format("<< %s", name.cString());

	ILogManager::instance().logMessage(LogMode::Normal, m_begin);
	ILogManager::instance().blockBegin();
}

inline
cut::LogBlock::~LogBlock()
{
	ILogManager::instance().blockEnd();
	ILogManager::instance().logMessage(LogMode::Normal, m_end);
}
