#include "stdafx.h"
#include "cut/testing/unit-test-settings.h"
#include <string.h>

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

    testManager.initializeFunction() = []{ lcpp::startup(); };
    testManager.shutdownFunction() = []{ lcpp::shutdown(); };

    testManager.disableUnitTestOrGroup("Environment", "add");
    testManager.disableUnitTestOrGroup("Environment", "set");

    testManager.runAll();

    auto& stats = testManager.statistics();

    if(g_pauseBeforeExit && stats.testsFailed > 0)
    {
        system("pause");
    }
    return 0;
}
