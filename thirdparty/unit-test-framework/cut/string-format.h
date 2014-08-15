#pragma once

#include "cut/string-ref.h"

namespace cut
{
	CUT_API std::string format(CUT_PRINTF_FORMAT_STRING StringRef fmt, ...);
}
