#pragma once

namespace cut
{
	struct UnitTestStatistics
	{
		std::size_t groups;
		std::size_t groupsFailed;
		std::size_t groupsDisabled;
		std::size_t tests;
		std::size_t testsFailed;
		std::size_t testsDisabled;
		std::size_t testsNotImplemented;
	};
}
