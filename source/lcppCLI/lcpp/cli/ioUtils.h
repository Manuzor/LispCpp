#pragma once

namespace lcpp
{
    ezResult findDirectory(const char* szDirName, const char* szStartPath, String* out_pParentPath);

    String getCurrentWorkingDirectory();
}
