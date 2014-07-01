#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    struct A
    {
        A(ezInt32 val) : val(val) {}

        virtual ezInt32 method()
        {
            return 1;
        }

        virtual ezInt32 constMethod() const
        {
            return 10;
        }

        ezInt32 val;
    };
    
    struct B : public A
    {
        B(ezInt32 val) : A(val) {}

        virtual ezInt32 method() override
        {
            return 2;
        }

        virtual ezInt32 constMethod() const override
        {
            return 20;
        }
    };

    UnitTestGroup g_group("RefTests");

    UnitTest g_test1(g_group, "Ptr", []{
        A a(42);
        B b(1337);
        Ptr<A> pA;
        Ptr<B> pB;

        CUT_ASSERT.isTrue(pA.get() == nullptr);
        CUT_ASSERT.isTrue(pB.get() == nullptr);

        CUT_ASSERT.isTrue(pA.isNull());
        CUT_ASSERT.isFalse(pA);
        CUT_ASSERT.isTrue(!pA);
        CUT_ASSERT.isTrue(pB.isNull());
        CUT_ASSERT.isFalse(pB);
        CUT_ASSERT.isTrue(!pB);

        pA = &a;
        CUT_ASSERT.isFalse(pA.isNull());
        CUT_ASSERT.isTrue(pA);
        CUT_ASSERT.isTrue(pA.get() == &a);
        CUT_ASSERT.isTrue(pA->val == 42);
        CUT_ASSERT.isTrue((*pA).val == 42);

        pB = &b;
        CUT_ASSERT.isFalse(pB.isNull());
        CUT_ASSERT.isTrue(pB);
        CUT_ASSERT.isTrue(pB.get() == &b);
        CUT_ASSERT.isTrue(pB->val == 1337);
        CUT_ASSERT.isTrue((*pB).val == 1337);

        CUT_ASSERT.isTrue(pA->method() == 1);
        CUT_ASSERT.isTrue(pB->method() == 2);

        CUT_ASSERT.isTrue(cast<A>(pB)->method() == 2);
        CUT_ASSERT.isTrue(pB.cast<A>()->method() == 2);

        Ptr<B> pBCopy(pB);
        CUT_ASSERT.isFalse(pB.isNull());
        CUT_ASSERT.isFalse(pBCopy.isNull());
        CUT_ASSERT.isTrue(pBCopy.get() == pB.get());

        Ptr<B> pBMoved(std::move(pB));
        CUT_ASSERT.isTrue(pB.isNull());
        CUT_ASSERT.isFalse(pBMoved.isNull());
        CUT_ASSERT.isTrue(pBCopy.get() == pBCopy.get());

        Ptr<B> pBCopy2;
        CUT_ASSERT.isTrue(pBCopy2.isNull());
        pBCopy2 = pBCopy;
        CUT_ASSERT.isFalse(pBCopy2.isNull());
        CUT_ASSERT.isTrue(pBCopy2.get() == pBCopy.get());

        pB = std::move(pBMoved);
        CUT_ASSERT.isFalse(pB.isNull());
        CUT_ASSERT.isTrue(pBMoved.isNull());
        CUT_ASSERT.isTrue(pB.get() == pBCopy.get());

        const A constA(123);
        Ptr<const A> pConstA(&constA);

        auto pConstB = pConstA.cast<const B>();
        CUT_ASSERT.isTrue(pConstB->constMethod() == pConstA->constMethod());

        Ptr<A> pAConstruct(pB);
        CUT_ASSERT.isTrue(pAConstruct.get() == pB.get());
        CUT_ASSERT.isTrue(pAConstruct == pB);

        Ptr<A> pAAssign(nullptr);
        pAAssign = pB;
    });
}
