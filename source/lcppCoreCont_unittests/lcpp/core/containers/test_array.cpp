#include "stdafx.h"
#include "lcpp/core/containers/array.h"

namespace lcpp
{
    namespace test
    {
        struct StaticCounter
        {
            static std::size_t s_uiConstructionCount;
            static std::size_t s_uiDestructionCount;

            static void reset()
            {
                s_uiConstructionCount = 0;
                s_uiDestructionCount = 0;
            }

            static bool check(std::size_t expectedConstructions, std::size_t expectedDestructions)
            {
                return expectedConstructions == s_uiConstructionCount
                    && expectedDestructions == s_uiDestructionCount;
            }

            StaticCounter() : m_data(0) { ++s_uiConstructionCount; }
            StaticCounter(int data) : m_data(data) { ++s_uiConstructionCount; }
            ~StaticCounter() { ++s_uiDestructionCount; }

            int m_data;
        };
    }
}

std::size_t lcpp::test::StaticCounter::s_uiConstructionCount = 0;
std::size_t lcpp::test::StaticCounter::s_uiDestructionCount = 0;

LCPP_TestGroup(Array);

LCPP_TestCase(Array, Basics)
{
    Array<StaticCounter> arr;

    CUT_ASSERT.isTrue(StaticCounter::check(0, 0));

    //////////////////////////////////////////////////////////////////////////

    StaticCounter staticArray[10];
    CUT_ASSERT.isTrue(StaticCounter::check(10, 0));

    // Construct from static array
    Array<StaticCounter> arr2(staticArray);

    CUT_ASSERT.isTrue(arr2.getData() == staticArray);
    CUT_ASSERT.isTrue(arr2.getSize() == 10);

    // Assignment operator
    arr = staticArray;

    CUT_ASSERT.isTrue(arr.getData() == staticArray);
    CUT_ASSERT.isTrue(arr.getSize() == 10);

    // Assignment
    arr.assign(staticArray);

    CUT_ASSERT.isTrue(arr.getData() == staticArray);
    CUT_ASSERT.isTrue(arr.getSize() == 10);

    //////////////////////////////////////////////////////////////////////////

    StaticCounter* plainArray = new StaticCounter[42];
    CUT_ASSERT.isTrue(StaticCounter::check(10 + 42, 0));

    // Construct from dynamically allocated array
    Array<StaticCounter> arr3(plainArray, 42);

    CUT_ASSERT.isTrue(arr3.getData() == plainArray);
    CUT_ASSERT.isTrue(arr3.getSize() == 42);

    // Assignment
    arr.assign(plainArray, 42);

    //////////////////////////////////////////////////////////////////////////
    
    CUT_ASSERT.isTrue(StaticCounter::check(10 + 42, 0));

    arr.reset();
    CUT_ASSERT.isTrue(arr.getData() == nullptr);
    CUT_ASSERT.isTrue(arr.getSize() == 0);
}

LCPP_TestCase(Array, Index)
{
    auto szHello("0123456789");

    Array<const char> arr(szHello, 10);

    CUT_ASSERT.isTrue(arr[0] == '0');
    CUT_ASSERT.isTrue(arr[1] == '1');
    CUT_ASSERT.isTrue(arr[2] == '2');
    CUT_ASSERT.isTrue(arr[3] == '3');
    CUT_ASSERT.isTrue(arr[4] == '4');
    CUT_ASSERT.isTrue(arr[5] == '5');
    CUT_ASSERT.isTrue(arr[6] == '6');
    CUT_ASSERT.isTrue(arr[7] == '7');
    CUT_ASSERT.isTrue(arr[8] == '8');
    CUT_ASSERT.isTrue(arr[9] == '9');
    //CUT_ASSERT.isTrue(arr[10] == '\0'); // Out of bounds access.
}

namespace lcpp
{
    namespace test
    {
        static bool isEqual_NonZeroTerminated(const char* lhs, const char* rhs, std::size_t count)
        {
            for (std::size_t i = 0; i < count; ++i)
            {
                if (lhs[i] != rhs[i])
                {
                    return false;
                }
            }

            return true;
        }

        struct SomeData
        {
            std::size_t m_id;
            void* m_data0;
            void* m_data1;
            void* m_data2;
            void* m_data3;

            SomeData(std::size_t id) : m_id(id), m_data0(nullptr), m_data1(nullptr), m_data2(nullptr), m_data3(nullptr) {}
        };
    }
}

LCPP_TestCase(Array, SubArray)
{
    {
        auto szHello("Hello World.");

        Array<const char> arr(szHello, 12);

        CUT_ASSERT.isTrue(arr == arr.getSubArray(0));
        CUT_ASSERT.isTrue(arr.getSubArray(0) == arr(0));

        auto sub = arr(6);
        CUT_ASSERT.isTrue(sub.getSize() == 6);
        CUT_ASSERT.isTrue(isEqual_NonZeroTerminated(sub.getData(), "World.", 6));

        sub = arr(6, 5);
        CUT_ASSERT.isTrue(sub.getSize() == 5);
        CUT_ASSERT.isTrue(isEqual_NonZeroTerminated(sub.getData(), "World", 5));

        sub = arr(11, 1);
        CUT_ASSERT.isTrue(sub.getSize() == 1);
        CUT_ASSERT.isTrue(*sub.getData() == '.');

        sub = arr(0, 1);
        CUT_ASSERT.isTrue(sub.getSize() == 1);
        CUT_ASSERT.isTrue(sub.getData() == arr.getData());
        CUT_ASSERT.isTrue(*sub.getData() == 'H');

        sub = arr(0, 0);
        CUT_ASSERT.isTrue(sub.getSize() == 0);
        CUT_ASSERT.isTrue(sub.getData() == nullptr);
    }

    {
        SomeData content[] = { SomeData(0), SomeData(1), SomeData(2) };

        Array<SomeData> arr(content);

        CUT_ASSERT.isTrue(arr(0)[0].m_id == 0);
        CUT_ASSERT.isTrue(arr(0)[1].m_id == 1);
        CUT_ASSERT.isTrue(arr(0)[2].m_id == 2);

        CUT_ASSERT.isTrue(arr(1)[0].m_id == 1);
        CUT_ASSERT.isTrue(arr(1)[1].m_id == 2);

        CUT_ASSERT.isTrue(arr(2)[0].m_id == 2);

        //CUT_ASSERT.isTrue(arr(3)[0].m_id == ???); // Out of bounds.
    }
}
