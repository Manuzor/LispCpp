#bin/sh

echo "#ifndef LCPP_CORE_CONT_INCLUDE_ALL
#error Do not include this file directly in your project! It is meant for internal use only.
#endif

#include \"stdafx.h\"

$(find . | grep 'lcpp/core/.*\.h' | sed -e 's:\./\(.*\):#include "\1":' -)" > includeAll.cpp
