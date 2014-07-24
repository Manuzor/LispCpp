#pragma once

namespace cut
{
	inline const std::string& emptyString()
	{
		static std::string str("");
		return str;
	}
}
