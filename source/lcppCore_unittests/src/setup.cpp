#include "stdafx.h"

namespace lcpp { namespace unittests {

    TEST_MODULE_INITIALIZE(lcpp_Initialize)
    {
        ezStartup::StartupCore();
    }

    TEST_MODULE_CLEANUP(lcpp_Cleanup)
    {
        ezStartup::ShutdownBase();
    }

}}