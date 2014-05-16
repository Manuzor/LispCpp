#include "stdafx.h"
#include "lcpp/api_inl.h"
#include "cut/testing/unit-test-settings.h"
#include <string.h>

void processArgument(const char* arg)
{
    if(strcmp(arg, "-i") == 0 || strcmp(arg, "--interactive") == 0)
    {
        cut::settings().doDebugBreaks = true;
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

    testManager.initializeFunction() = [](){ ezStartup::StartupCore(); };
    testManager.shutdownFunction() = [](){ ezStartup::ShutdownBase(); };

    testManager.runAll();

    system("pause");
    return 0;
}
