#include "stdafx.h"

namespace
{
    class TestCollectable :
        public lcpp::CollectableBase
    {
    public:

        TestCollectable(ezInt32 iData) : m_iData(iData) {}

        ezInt32 m_iData;
    };
}

LCPP_TestGroup(StackPtr);

LCPP_TestCaseNoInit(StackPtr, Basics)
{
    TestCollectable t1(42);
    TestCollectable t2(1337);

    CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 0);
    CUT_ASSERT.isTrue(t1.m_iData == 42);
    CUT_ASSERT.isTrue(t2.m_iData == 1337);

    {
        StackPtr<TestCollectable> st1 = &t1;
        CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 1);
        CUT_ASSERT.isTrue(st1->m_iData == 42);

        {
            auto copySt1 = st1;
            CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 2);
            CUT_ASSERT.isTrue(st1->m_iData == 42);
            CUT_ASSERT.isTrue(copySt1->m_iData == 42);
        }

        CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 1);
        CUT_ASSERT.isTrue(st1->m_iData == 42);

        {
            auto copy1St1 = st1;
            auto copy2St1 = st1;
            auto copy3St1 = st1;

            CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 1 + 3);
            CUT_ASSERT.isTrue(st1->m_iData == 42);
            CUT_ASSERT.isTrue(copy1St1->m_iData == 42);
            CUT_ASSERT.isTrue(copy2St1->m_iData == 42);
            CUT_ASSERT.isTrue(copy3St1->m_iData == 42);

            StackPtr<TestCollectable> st2 = &t2;
            CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 1 + 3 + 1);
            CUT_ASSERT.isTrue(st2->m_iData == 1337);

            {
                auto copySt2 = st2;
                CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 1 + 3 + 1 + 1);
                CUT_ASSERT.isTrue(st1->m_iData == 42);
                CUT_ASSERT.isTrue(copy1St1->m_iData == 42);
                CUT_ASSERT.isTrue(copy2St1->m_iData == 42);
                CUT_ASSERT.isTrue(copy3St1->m_iData == 42);
                CUT_ASSERT.isTrue(st2->m_iData == 1337);
                CUT_ASSERT.isTrue(copySt2->m_iData == 1337);
            }

            CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 1 + 3 + 1);
            CUT_ASSERT.isTrue(st1->m_iData == 42);
            CUT_ASSERT.isTrue(copy1St1->m_iData == 42);
            CUT_ASSERT.isTrue(copy2St1->m_iData == 42);
            CUT_ASSERT.isTrue(copy3St1->m_iData == 42);
            CUT_ASSERT.isTrue(st2->m_iData == 1337);
        }


        CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 1);
        CUT_ASSERT.isTrue(st1->m_iData == 42);
    }

    CUT_ASSERT.isTrue(StackPtrBase::s_uiNextIndex == 0);
    CUT_ASSERT.isTrue(t1.m_iData == 42);
    CUT_ASSERT.isTrue(t2.m_iData == 1337);
}
