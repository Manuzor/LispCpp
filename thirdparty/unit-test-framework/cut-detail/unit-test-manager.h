#pragma once
#include "cut/testing/unit-test-manager.h"
#include "cut/testing/unit-test-statistics.h"

namespace cut
{
	typedef std::vector<IUnitTestGroup*> UnitTestGroupContainer;

	class DefaultUnitTestManager : public IUnitTestManager
	{
	public:

		DefaultUnitTestManager();
		virtual ~DefaultUnitTestManager();

		virtual Lambda_t& initializeFunction() CUT_OVERRIDE;
		virtual Lambda_t& shutdownFunction() CUT_OVERRIDE;

		virtual void registerUnitTestGroup(IUnitTestGroup* testGroup) CUT_OVERRIDE;

		virtual void run(StringRef groupName, StringRef testName) CUT_OVERRIDE;
		virtual void runAll() CUT_OVERRIDE;

		virtual void updateStatistics() CUT_OVERRIDE;
		virtual UnitTestStatistics& statistics() CUT_OVERRIDE;
		virtual const UnitTestStatistics& statistics() const CUT_OVERRIDE;
		virtual void printStatistics() const CUT_OVERRIDE;

		virtual void disableUnitTestOrGroup(StringRef groupName) CUT_OVERRIDE;
		virtual void disableUnitTestOrGroup(StringRef groupName, StringRef testName) CUT_OVERRIDE;

		virtual void enableUnitTestOrGroup(StringRef groupName) CUT_OVERRIDE;
		virtual void enableUnitTestOrGroup(StringRef groupName, StringRef testName) CUT_OVERRIDE;

		virtual bool isUnitTestOrGroupEnabled(StringRef groupName) CUT_OVERRIDE;
		virtual bool isUnitTestOrGroupEnabled(StringRef groupName, StringRef testName) CUT_OVERRIDE;

	private:
		Lambda_t m_initialize;
		Lambda_t m_shutdown;
		UnitTestGroupContainer m_unitTestGroups;
		UnitTestStatistics m_statistics;

		std::vector<std::string> m_disabledUnitTestGroups;
		std::multimap<std::string, std::string> m_disabledUnitTests;
	};
}
