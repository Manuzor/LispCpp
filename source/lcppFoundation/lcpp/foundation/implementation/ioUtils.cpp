#include "stdafx.h"
#include "lcpp/foundation/ioUtils.h"

#if EZ_ENABLED(EZ_PLATFORM_WINDOWS)
  #include "lcpp/foundation/implementation/ioUtils_win.h"
#else
  #pragma error "Unsupported platform"
#endif
