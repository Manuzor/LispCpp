
#include "cut/string-format.h"

inline
cut::LogBlock::LogBlock(StringRef name) :
	m_name(name.cString())
{
	ILogManager::instance().blockBegin(m_name);
}

inline
cut::LogBlock::~LogBlock()
{
	ILogManager::instance().blockEnd(m_name);
}
