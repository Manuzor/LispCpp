#include "stdafx.h"

#include "cut/testing/unit-test-settings.h"
#include "cut/logging/log-manager.h"
#include "cut/logging/default-loggers.h"

#include "lcpp/core/runtime.h"

namespace cut
{
    namespace loggers
    {
        struct ezEngineWriter
        {
            ezLogInterface* m_pInterface;
            ezHybridArray<ezLogBlock*, 32, lcpp::AllocatorWrapper_Default> m_logBlocks;

            ezEngineWriter(ILogManager& logManager, ezLogInterface* pInterface) :
                m_pInterface(pInterface)
            {
                EZ_ASSERT(pInterface, "Invalid ptr.");

                m_logBlocks.Reserve(32);

                using namespace std::placeholders;
                logManager.addLoggerFunction(std::bind(&ezEngineWriter::logMessageHandler, this, _1));
                logManager.addBlockListener(std::bind(&ezEngineWriter::blockHandler, this, _1));
            }

            ~ezEngineWriter()
            {
                m_logBlocks.Clear();
                m_pInterface = nullptr;
            }

            EZ_FORCE_INLINE
            void logMessageHandler(const LoggerInfo& loggerInfo)
            {
                switch(loggerInfo.logMode)
                {
                case LogMode::Normal:
                    ezLog::Info(m_pInterface, loggerInfo.message.cString());
                    break;
                case LogMode::Success:
                    ezLog::Success(m_pInterface, loggerInfo.message.cString());
                    break;
                case LogMode::Failure:
                    ezLog::Error(m_pInterface, loggerInfo.message.cString());
                    break;
                case LogMode::Warning:
                    ezLog::Warning(m_pInterface, loggerInfo.message.cString());
                    break;
                default:
                    EZ_REPORT_FAILURE("Unknown LogMode.");
                    break;
                }
            }

            EZ_FORCE_INLINE
            void blockHandler(const LogBlockInfo& blockInfo)
            {
                switch(blockInfo.action)
                {
                case LogBlockAction::Begin:
                {
                    auto pBlock = EZ_NEW(lcpp::defaultAllocator(), ezLogBlock) { m_pInterface, blockInfo.name.cString() };
                    m_logBlocks.PushBack(pBlock);
                    break;
                }
                case LogBlockAction::End:
                {
                    EZ_ASSERT(m_logBlocks.GetCount() > 0, "Invalid state of the LogBlock stack.");
                    
                    auto& pBlock = m_logBlocks.PeekBack();
                    EZ_DELETE(lcpp::defaultAllocator(), pBlock);
                    m_logBlocks.PopBack();
                    break;
                }
                default:
                    EZ_REPORT_FAILURE("Unknown LogBlockAction.");
                    ezLog::Warning("Unknown LogBlockAction.");
                    break;
                }
            }
        };
    }
}

bool g_pauseBeforeExit = true;
ezLogWriter::HTML* g_pHtmlLog = nullptr;
cut::loggers::ezEngineWriter* g_pEzEngineLogger = nullptr;

void processArgument(const char* arg)
{
    if(strcmp(arg, "-i") == 0 || strcmp(arg, "--interactive") == 0)
    {
        cut::settings().doDebugBreaks = true;
    }
    else if(strcmp(arg, "--no-pause") == 0)
    {
        g_pauseBeforeExit = false;
    }
}

int main(int argc, const char* argv[])
{
    // parse settings
    for(int i = 1; i < argc; ++i)
    {
        processArgument(argv[i]);
    }

    auto& testManager = cut::IUnitTestManager::instance();

    testManager.initializeFunction() = []
    {
        lcpp::startup();

        g_pEzEngineLogger = EZ_NEW(lcpp::defaultAllocator(), cut::loggers::ezEngineWriter)(cut::ILogManager::instance(),
                                                                                           ezLog::GetDefaultLogSystem());
        g_pHtmlLog = EZ_NEW(lcpp::defaultAllocator(), ezLogWriter::HTML)();

        ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);

        ezGlobalLog::AddLogWriter(ezLogWriter::Console::LogMessageHandler);
        ezGlobalLog::AddLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);

        ezStringBuilder testDir;
        lcpp::getCurrentWorkingDirectory(testDir);

        ezStringBuilder logFile;
        logFile.AppendPath("temp", "log");
        logFile.MakeAbsolutePath(testDir.GetData());
        logFile.MakeCleanPath();
        ezOSFile::CreateDirectoryStructure(logFile.GetData());
        if (ezFileSystem::AddDataDirectory(logFile.GetData(), ezFileSystem::AllowWrites, "log-dir").Failed())
        {
            return;
        }

        ezGlobalLog::AddLogWriter(ezLoggingEvent::Handler(&ezLogWriter::HTML::LogMessageHandler, g_pHtmlLog));
        logFile.AppendPath("lcppCoreCont_unittests.log.html");
        g_pHtmlLog->BeginLog(logFile.GetFileNameAndExtension().GetData(), logFile.GetFileNameAndExtension().GetData());

        testDir.AppendPath("data1", "test");
        testDir.MakeCleanPath();
        auto addingDataDirectory = ezFileSystem::AddDataDirectory(testDir.GetData(), ezFileSystem::ReadOnly, "test-data");
        if(addingDataDirectory.Failed())
        {
            testDir.Prepend("Failed to add test data directory: ");
            throw std::exception(testDir.GetData());
        }
        
        LCPP_test_pRuntimeState->setUserDirectory(testDir.GetData());
    };
    testManager.shutdownFunction() = []
    {
        ezGlobalLog::RemoveLogWriter(ezLoggingEvent::Handler(&ezLogWriter::HTML::LogMessageHandler, g_pHtmlLog));
        ezGlobalLog::RemoveLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);
        ezGlobalLog::RemoveLogWriter(ezLogWriter::Console::LogMessageHandler);

        g_pHtmlLog->EndLog();

        EZ_DELETE(lcpp::defaultAllocator(), g_pHtmlLog);
        EZ_DELETE(lcpp::defaultAllocator(), g_pEzEngineLogger);

        lcpp::shutdown();
    };

    testManager.runAll();

    auto& stats = testManager.statistics();

    if(g_pauseBeforeExit && stats.testsFailed > 0)
    {
        system("pause");
    }
    return 0;
}
