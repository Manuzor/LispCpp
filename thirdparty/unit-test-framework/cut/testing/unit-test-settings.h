#pragma once

namespace cut
{
	struct UnitTestSettings
	{
	public:
		bool doDebugBreaks;

		UnitTestSettings() :
			doDebugBreaks(false)
		{
		}
	};
	
	CUT_API UnitTestSettings& settings();
}
