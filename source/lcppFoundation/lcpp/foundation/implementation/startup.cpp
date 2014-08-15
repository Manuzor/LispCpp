#include "stdafx.h"
#include "lcpp/foundation/startup.h"

void lcpp::startup()
{
    ezStartup::StartupCore();
    ezGlobalEvent::Broadcast("LCPP_GLOBAL_EVENT_STARTUP");
}

void lcpp::shutdown()
{
    ezGlobalEvent::Broadcast("LCPP_GLOBAL_EVENT_SHUTDOWN");
    ezStartup::ShutdownCore();
}
