#include "cut/logging/log-manager.h"

CUT_FORCE_INLINE
cut::loggers::StdOutWriter::StdOutWriter(ILogManager& logManager)
{
	using namespace std::placeholders;
	logManager.registerLoggerFunction(std::bind(&StdOutWriter::logMessageHandler, this, _1, _2));
}

#ifdef _WIN32

CUT_FORCE_INLINE
void
cut::loggers::StdOutWriter::logMessageHandler(LogMode mode, StringRef formattedMessage)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO initialConsoleState;
	GetConsoleScreenBufferInfo(hStdOut, &initialConsoleState);

	switch(mode)
	{
	case LogMode::Normal:
		SetConsoleTextAttribute(hStdOut, 0x07); // Regular
		break;
	case LogMode::Warning:
		SetConsoleTextAttribute(hStdOut, 0x0E); // Yellow
		break;
	case LogMode::Failure:
		SetConsoleTextAttribute(hStdOut, 0x0C); // Red
		break;
	case LogMode::Success:
		SetConsoleTextAttribute(hStdOut, 0x0A); // Green
		break;
	default:
		debugBreak();
		break;
	}

	printf(formattedMessage.cString());

	// Restore the default.
	SetConsoleTextAttribute(hStdOut, initialConsoleState.wAttributes);
}

#else

CUT_FORCE_INLINE
void
cut::loggers::StdOutWriter::logMessageHandler(LogMode mode, StringRef formattedMessage)
{
	printf(formattedMessage.cString());
}

#endif // _WIN32

//////////////////////////////////////////////////////////////////////////

CUT_FORCE_INLINE
cut::loggers::FileWriter::FileWriter(ILogManager& logManager, StringRef fileName) :
	m_file(fileName.cString())
{
	using namespace std::placeholders;
	logManager.registerLoggerFunction(std::bind(&FileWriter::logMessageHandler, this, _1, _2));
}

CUT_FORCE_INLINE
cut::loggers::FileWriter::~FileWriter()
{
	m_file.close();
}

CUT_FORCE_INLINE
void cut::loggers::FileWriter::logMessageHandler(LogMode mode, StringRef formattedMessage)
{
	m_file << formattedMessage.cString();
}

//////////////////////////////////////////////////////////////////////////

CUT_FORCE_INLINE
cut::loggers::VisualStudioWriter::VisualStudioWriter(ILogManager& logManager)
{
	using namespace std::placeholders;
	logManager.registerLoggerFunction(std::bind(&VisualStudioWriter::logMessageHandler, this, _1, _2));
}

#ifdef _WIN32

CUT_FORCE_INLINE
void cut::loggers::VisualStudioWriter::logMessageHandler(LogMode mode, StringRef formattedMessage)
{
	OutputDebugString(formattedMessage.cString());
}

#else

CUT_FORCE_INLINE
void cut::loggers::VisualStudioWriter::logMessageHandler(LogMode mode, StringRef formattedMessage)
{
}

#endif
