#pragma once

namespace lcpp
{
    /// \brief Stores the current working directory in \a out_path.
    /// \remark Will clear \a out_path before anything else happens.
    LCPP_API_FOUNDATION void getCurrentWorkingDirectory(ezStringBuilder& out_path);
}
