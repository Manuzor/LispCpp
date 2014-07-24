#include "cut/testing/unit-test-group.h"

inline
cut::UnitTest::UnitTest(IUnitTestGroup& testGroup, StringRef testName, Lambda_t lambda) :
	m_name(testName),
	m_lambda(lambda)
{
	testGroup.registerUnitTest(this);
}
