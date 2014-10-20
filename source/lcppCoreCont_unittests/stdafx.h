#pragma once

#include <SDKDDKVer.h>


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <limits>
#include <functional>

#include <windows.h>

#include "cut/api.h"

#include "lcpp/foundation/api.h"
#include "lcpp/core/api.h"

#include "lcpp/test/common.h"

#include <Foundation/IO/FileSystem/DataDirTypeFolder.h>
#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/VisualStudioWriter.h>
#include <Foundation/Logging/HTMLWriter.h>
