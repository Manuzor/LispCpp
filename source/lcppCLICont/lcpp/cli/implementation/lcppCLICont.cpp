// lcppCLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <functional>
#include "lcpp/cli/exceptions.h"
#include "lcpp/cli/interpreter.h"
#include "lcpp/cli/ioUtils.h"
#include <Foundation/Utilities/CommandLineUtils.h>

namespace lcpp
{
    class LoggingSystem
    {
        ezStringBuilder m_absoluteLogFilesDir;
        ezLogWriter::HTML m_htmlLog;
    public:
        LoggingSystem(const char* logFilesDirectory) :
            m_absoluteLogFilesDir()
        {
            getCurrentWorkingDirectory(m_absoluteLogFilesDir);
            m_absoluteLogFilesDir.AppendPath(logFilesDirectory);
            m_absoluteLogFilesDir.MakeCleanPath();
        }

        void initialize()
        {
            // set up console and visual studio loggers.
            ezGlobalLog::AddLogWriter(ezLogWriter::Console::LogMessageHandler);
            ezGlobalLog::AddLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);

            // set up html file log.
            ezOSFile::CreateDirectoryStructure(m_absoluteLogFilesDir.GetData());

            ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);
            ezFileSystem::AddDataDirectory(m_absoluteLogFilesDir.GetData(), ezFileSystem::AllowWrites, "loggingDirectory");

            ezGlobalLog::AddLogWriter(ezLoggingEvent::Handler(&ezLogWriter::HTML::LogMessageHandler, &m_htmlLog));

            m_absoluteLogFilesDir.AppendPath(g_ApplicationTitleShort);
            m_absoluteLogFilesDir.Append(".log.html");
            m_htmlLog.BeginLog(m_absoluteLogFilesDir.GetData(), g_ApplicationTitle);

            m_absoluteLogFilesDir.Clear();
        }

        void shutdown()
        {
            ezGlobalLog::RemoveLogWriter(ezLoggingEvent::Handler(&ezLogWriter::HTML::LogMessageHandler, &m_htmlLog));
            ezGlobalLog::RemoveLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);
            ezGlobalLog::RemoveLogWriter(ezLogWriter::Console::LogMessageHandler);

            m_htmlLog.EndLog();

            ezFileSystem::RemoveDataDirectoryGroup("loggingDirectory");
            ezFileSystem::ClearAllDataDirectoryFactories(); //TODO: Check if this is ok to be here.
        }

        EZ_DISALLOW_COPY_AND_ASSIGN(LoggingSystem);
    };
}

ezInt32 run()
{
    using namespace lcpp;

    auto interpreter = Interpreter();
    interpreter.initialize();

    auto result = interpreter.repl();
    interpreter.shutdown();

    return result;
}

void parseCommandLineArgs(int argc, const char* argv[])
{
    if(argc == 1) { return; }

    ezCommandLineUtils cmd;
    cmd.SetCommandLine(argc, argv);

    if (cmd.GetBoolOption("-v"))
    {
        ezGlobalLog::SetLogLevel(ezLogMsgType::All);
    }

    if (cmd.GetBoolOption("-q"))
    {
        ezGlobalLog::SetLogLevel(ezLogMsgType::None);
    }

    ezStringBuilder sCurrentArg;
    for(int i = 1; i < argc; ++i)
    {
        sCurrentArg = argv[i];

        if (sCurrentArg.IsEqual("-v"))
        {
        }
        else if (sCurrentArg.IsEqual("-q"))
        {
            ezGlobalLog::SetLogLevel(ezLogMsgType::None);
        }
    }
}

int main(int argc, const char* argv[])
{
    lcpp::startup();
    LCPP_SCOPE_EXIT{ lcpp::shutdown(); };

    lcpp::LoggingSystem loggingSystem("temp/log/");
    loggingSystem.initialize();
    LCPP_SCOPE_EXIT { loggingSystem.shutdown(); };

    parseCommandLineArgs(argc, argv);

    EZ_LOG_BLOCK("ezEngine running.");

    //ezStartup::PrintAllSubsystems();
    ezInt32 status = 0;

    try
    {
        status = run();
    }
    catch (lcpp::exceptions::ExceptionBase& e)
    {
        ezLog::Error(e.what());
        EZ_REPORT_FAILURE("Uncaught exception!");
        return -1;
    }
    catch (std::exception& e)
    {
        ezLog::Error(e.what());
        EZ_REPORT_FAILURE("Uncaught std::exception!");
        return -2;
    }
    catch(...)
    {
        ezLog::Error("Something was thrown and it was not caught!");
        EZ_REPORT_FAILURE("Something was thrown and it was not caught!");
        return -4;
    }

    return status;
}
