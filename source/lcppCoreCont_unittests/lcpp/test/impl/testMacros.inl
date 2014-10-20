
// The arguments are not strings, use them like this: LCPP_TestCase(Hello, World)
#define LCPP_TestCase(groupName, testName)                      \
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

// The arguments are not strings, use them like this: LCPP_TestCase(Hello, World)
#define LCPP_TestCaseNoInit(groupName, testName)                \
namespace lcpp { namespace test {                               \
    class UnitTestNoInit_##groupName##testName :                \
        public UnitTestNoInit                                   \
    {                                                           \
    public:                                                     \
                                                                \
        UnitTestNoInit_##groupName##testName() :                \
            UnitTestNoInit(g_UnitTestGroup_##groupName) {}      \
                                                                \
        virtual void run() override;                            \
                                                                \
        virtual const cut::StringRef getName() const override   \
        {                                                       \
            return #testName;                                   \
        }                                                       \
    };                                                          \
}}                                                              \
lcpp::test::UnitTestNoInit_##groupName##testName g_UnitTestNoInit_##groupName##testName; \
void lcpp::test::UnitTestNoInit_##groupName##testName::run()
