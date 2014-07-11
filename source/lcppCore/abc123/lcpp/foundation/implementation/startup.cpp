#include "stdafx.h"
#include "lcpp/foundation/startup.h"


LCPP_CORE_API void lcpp::startup()
{
    ezStartup::StartupCore();
}

LCPP_CORE_API void lcpp::shutdown()
{
    ezStartup::ShutdownBase();
}
