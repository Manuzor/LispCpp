
// The argument is not a string, use it like this: LCPP_TEST_GROUP(Hello)
#define LCPP_TestGroup(groupName)                          \
namespace lcpp { namespace test {                          \
    class UnitTestGroup_##groupName :                      \
        public cut::UnitTestGroup                          \
    {                                                      \
    public:                                                \
        UnitTestGroup_##groupName() :                      \
            cut::UnitTestGroup(#groupName) {}              \
        cut::Lambda_t& getInitializationLambda()           \
        {                                                  \
            return m_initialization;                       \
        }                                                  \
        cut::Lambda_t& getShutdownLambda()                 \
        {                                                  \
            return m_shutdown;                             \
        }                                                  \
    };                                                     \
}}                                                         \
lcpp::test::UnitTestGroup_##groupName g_UnitTestGroup_##groupName

#define LCPP_TestGroup_Initialization(groupName) \
    lcpp::test::g_UnitTestGroup_##groupName::getInitializationLambda() = []

#define LCPP_TestGroup_Shutdown(groupName) \
    lcpp::test::g_UnitTestGroup_##groupName::getShutdownLambda() = []
