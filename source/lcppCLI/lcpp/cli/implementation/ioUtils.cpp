#include "stdafx.h"
#include "lcpp/cli/ioUtils.h"

ezResult
lcpp::findDirectory(const char* szDirName, const char* szStartPath, String* out_pParentPath)
{
    ezStringBuilder searchThis;
    auto parentPathProxy = ezString();
    ezFileSystem::ResolvePath(szStartPath, false, &parentPathProxy, nullptr);
    *out_pParentPath = parentPathProxy.GetData();
    searchThis.Append(out_pParentPath->GetData());
    searchThis.MakeCleanPath();

    EZ_ASSERT(searchThis.IsAbsolutePath(), "...");

    ezFileSystemIterator iter;

    for(ezUInt8 maxNumRetries = 10; maxNumRetries > 0; --maxNumRetries)
    {
        for(auto result = iter.StartSearch(searchThis.GetData(), false, true);
            result.Succeeded();
            result = iter.Next())
        {
            auto& stats = iter.GetStats();

            if (stats.m_bIsDirectory && stats.m_sFileName.IsEqual(szDirName))
            {
                // found!
                searchThis.AppendPath(szDirName);
                *out_pParentPath = searchThis;
                return EZ_SUCCESS;
            }
        }
        searchThis.PathParentDirectory();
    }

    return EZ_FAILURE;
}

#if EZ_ENABLED(EZ_PLATFORM_WINDOWS)
#include "lcpp/cli/implementation/ioUtils_win.h"
#else
#pragma error Unsupported platform
#endif
