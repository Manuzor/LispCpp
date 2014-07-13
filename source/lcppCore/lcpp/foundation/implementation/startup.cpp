#include "stdafx.h"
#include "lcpp/foundation/startup.h"
#include "lcpp/core/runtime.h"

void lcpp::startup()
{
    ezStartup::StartupCore();
    LispRuntime::instance()->initialize();
}

void lcpp::shutdown()
{
    LispRuntime::instance()->shutdown();
    ezStartup::ShutdownBase();
}
