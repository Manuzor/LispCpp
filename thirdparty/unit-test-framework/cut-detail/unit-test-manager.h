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

		virtual Lambda_t& initializeFunction() override;
		virtual Lambda_t& shutdownFunction() override;

		virtual void registerUnitTestGroup(IUnitTestGroup* testGroup) CUT_OVERRIDE;

		virtual void runAll() CUT_OVERRIDE;

		virtual void updateStatistics() CUT_OVERRIDE;
		virtual const UnitTestStatistics& statistics() const CUT_OVERRIDE;

	protected:
	private:
		Lambda_t m_initialize;
		Lambda_t m_shutdown;
		UnitTestGroupContainer m_unitTestGroups;
		UnitTestStatistics m_statistics;
	};
}
