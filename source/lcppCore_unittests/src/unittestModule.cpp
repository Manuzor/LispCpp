#include "stdafx.h"

namespace lcpp { namespace unittests {

    TEST_MODULE_INITIALIZE(SchemeTypeTests_Initialize)
    {
        ezStartup::StartupCore();
    }

    TEST_MODULE_CLEANUP(SchemeTypeTests_Cleanup)
    {
        ezStartup::ShutdownBase();
    }

}}