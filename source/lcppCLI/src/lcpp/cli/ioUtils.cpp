#include "stdafx.h"
#include "lcpp/cli/ioUtils.h"

ezResult lcpp::findDirectory(const char* szDirName, const char* szStartPath, ezString* out_pParentPath)
{
    ezStringBuilder searchThis;
    ezFileSystem::ResolvePath(szStartPath, false, out_pParentPath, nullptr);
    searchThis.Append(out_pParentPath->GetData());
    searchThis.MakeCleanPath();

    EZ_ASSERT(searchThis.IsAbsolutePath(), "...");

    ezFileSystemIterator iter;

    for(ezUInt8 maxNumRetries = 10; maxNumRetries > 0; --maxNumRetries)
    {
        for(auto result = iter.StartSearch(searchThis.GetData(), false, true);
            result.IsSuccess();
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
