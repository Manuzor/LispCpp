// lcppCLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <functional>
#include "lcpp/cli/exceptions.h"
#include "lcpp/cli/interpreter.h"

namespace lcpp
{
    class LoggingSystem
    {
        ezStringBuilder m_absoluteLogFilesDir;
        ezLogWriter::HTML m_htmlLog;
    public:
        LoggingSystem(const char* logFilesDirectory) :
            m_absoluteLogFilesDir(logFilesDirectory)
        {
            m_absoluteLogFilesDir.MakeAbsolutePath(ezOSFile::GetApplicationDirectory());
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

void run()
{
    using namespace lcpp;
    // Factory
    TypeFactory factory;

    // Reader
    Reader::CInfo readerCinfo;
    readerCinfo.pFactory = &factory;
    Reader reader(readerCinfo);

    // Evaluator
    RecursiveEvaluator::CInfo evalCinfo;
    evalCinfo.pFactory = &factory;
    RecursiveEvaluator evaluator(evalCinfo);

    // Printer
    Printer printer;
    
    Interpreter::CInfo cinfo;
    cinfo.pReader = &reader;
    cinfo.pEvaluator = &evaluator;
    cinfo.pPrinter = &printer;

    Interpreter interpreter(cinfo);

    interpreter.run();
}

int main(int argc, const char* argv[])
{
    lcpp::startup();
    LCPP_SCOPE_EXIT{ lcpp::shutdown(); };

    lcpp::LoggingSystem loggingSystem("log/");
    loggingSystem.initialize();
    LCPP_SCOPE_EXIT { loggingSystem.shutdown(); };

    EZ_LOG_BLOCK("ezEngine running.");

    //ezStartup::PrintAllSubsystems();

    try
    {
        run();
    }
    catch (lcpp::exceptions::ExceptionBase& e)
    {
        ezLog::Error(e.message());
        return -1;
    }
    catch (std::exception& e)
    {
        EZ_ASSERT(false, "Uncaught std::exception!");
        ezLog::Error(e.what());
        return -2;
    }
    catch(...)
    {
        EZ_ASSERT(false, "Something was thrown and it was not caught!");
        ezLog::Error("Something was thrown and it was not caught!");
        return -4;
    }

    return 0;
}
