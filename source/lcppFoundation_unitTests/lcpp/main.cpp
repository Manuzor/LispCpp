#include "stdafx.h"
#include "cut/testing/unit-test-settings.h"
#include <string.h>
#include "cut/testing/unit-test-manager.h"

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

    testManager.runAll();

    if(g_pauseBeforeExit)
    {
        system("pause");
    }
    return 0;
}
