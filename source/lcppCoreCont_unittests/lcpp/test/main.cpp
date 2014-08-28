#include "stdafx.h"
#include "cut/testing/unit-test-settings.h"
#include <string.h>
#include "lcpp/core/runtime.h"

bool g_pauseBeforeExit = true;

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


        ezGlobalLog::AddLogWriter(ezLogWriter::Console::LogMessageHandler);
        ezGlobalLog::AddLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);

        ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);

        ezStringBuilder testDir;
        lcpp::getCurrentWorkingDirectory(testDir);
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
        ezGlobalLog::RemoveLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);
        ezGlobalLog::RemoveLogWriter(ezLogWriter::Console::LogMessageHandler);

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
