
// The arguments are not strings, use them like this: LCPP_TEST(Hello, World)
#define LCPP_TestCase(groupName, testName)                          \
namespace lcpp { namespace test {                               \
    class UnitTest_##groupName##testName :                      \
        public UnitTest                                         \
    {                                                           \
    public:                                                     \
                                                                \
        UnitTest_##groupName##testName() :                      \
            UnitTest(g_UnitTestGroup_##groupName) {}            \
                                                                \
        virtual void doRun() override;                          \
                                                                \
        virtual const cut::StringRef getName() const override   \
        {                                                       \
            return #testName;                                   \
        }                                                       \
    };                                                          \
}}                                                              \
lcpp::test::UnitTest_##groupName##testName g_UnitTest_##groupName##testName;\
void lcpp::test::UnitTest_##groupName##testName::doRun()
