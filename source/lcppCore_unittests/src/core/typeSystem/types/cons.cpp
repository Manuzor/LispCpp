#include "stdafx.h"
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {

    TEST_CLASS(SchemeConsTests)
    {
        TEST_METHOD(Construction)
        {
            // Default construction
            {
                SchemeCons cons;
                Assert::AreEqual<SchemeObject>(cons.car(), SCHEME_NIL,L"Car is not nil!");
                Assert::AreEqual<SchemeObject>(cons.cdr(), SCHEME_NIL,L"Cdr is not nil!");
            }

            // Only car given
            {
                SchemeBool t(SCHEME_TRUE);
                SchemeCons cons(t);
                Assert::AreEqual<SchemeObject>(cons.car(), SCHEME_TRUE,L"Wrong object for car!");
                Assert::AreEqual<SchemeObject>(cons.cdr(), SCHEME_NIL,L"Cdr should be nil here!");
            }

            // Car and cdr explicitly given
            {
                SchemeBool t(SCHEME_TRUE);
                SchemeBool f(SCHEME_FALSE);
                SchemeCons cons(t, f);
                Assert::AreEqual<SchemeObject>(cons.car(), SCHEME_TRUE,L"Wrong car!");
                Assert::AreEqual<SchemeObject>(cons.cdr(), SCHEME_FALSE,L"Wrong cdr!");
            }
        }

        TEST_METHOD(CopyCtorAndCopyAssign)
        {
            {
                SchemeBool t(SCHEME_TRUE);

                SchemeCons first(t);
                SchemeCons second = first;
                Assert::AreEqual<SchemeObject>(first.car(), SCHEME_TRUE);
                Assert::AreEqual<SchemeObject>(second.car(), SCHEME_TRUE);
            }
            {
                SchemeInt32 theInt(42);
                SchemeCons first(theInt);
                SchemeCons second = first;
                Assert::AreEqual<SchemeObject>(first.car(), theInt);
                Assert::AreEqual<SchemeObject>(second.car(), theInt, L"Failed to construct 'second' by assigning it 'first'!");
                Assert::AreNotSame(first.car(), second.car(), L"Cons is not supposed to make a flat copy!");
            }
        }

        TEST_METHOD(ToString)
        {
            static SchemeInt32 one(1);
            static SchemeInt32 two(2);
            static SchemeInt32 three(3);

            struct TestWrapper
            {
                const char* expected;
                const SchemeCons cons;

                TestWrapper(const char* expected, const SchemeCons cons) :
                    expected(expected),
                    cons(cons)
                {
                }
            };

            ezDynamicArray<TestWrapper> tests;
            tests.Reserve(22);

            auto push = [&](const char* a, const SchemeCons& b)
            {
                tests.PushBack(TestWrapper(a, b));
            };

            push("(())",     SchemeCons(SCHEME_NIL, SCHEME_NIL));
            push("(1)",      SchemeCons(one, SCHEME_NIL));
            push("(() . 2)", SchemeCons(SCHEME_NIL, two));
            push("(1 . 2)",  SchemeCons(one, two));

            push("(() ())",     SchemeCons(SCHEME_NIL, SchemeCons(SCHEME_NIL, SCHEME_NIL)));
            push("(1 ())",      SchemeCons(one,        SchemeCons(SCHEME_NIL, SCHEME_NIL)));
            push("(1 2)",       SchemeCons(one,        SchemeCons(two,        SCHEME_NIL)));
            push("(1 () . 3)",  SchemeCons(one,        SchemeCons(SCHEME_NIL, three     )));
            push("(1 2 . 3)",   SchemeCons(one,        SchemeCons(two,        three     )));
            push("(() 2)",      SchemeCons(SCHEME_NIL, SchemeCons(two,        SCHEME_NIL)));
            push("(() () . 3)", SchemeCons(SCHEME_NIL, SchemeCons(SCHEME_NIL, three     )));
            push("(() 2 . 3)",  SchemeCons(SCHEME_NIL, SchemeCons(two,        three     )));
            push("(1 2 . 3)",   SchemeCons(one,        SchemeCons(two,        three     )));

            push("((()))",         SchemeCons(SchemeCons(SCHEME_NIL, SCHEME_NIL), SCHEME_NIL));
            push("((()) . 1)",     SchemeCons(SchemeCons(SCHEME_NIL, SCHEME_NIL), one       ));
            push("((2) . 1)",      SchemeCons(SchemeCons(two,        SCHEME_NIL), one       ));
            push("((() . 3) . 1)", SchemeCons(SchemeCons(SCHEME_NIL, three     ), one       ));
            push("((2 . 3) . 1)",  SchemeCons(SchemeCons(two,        three     ), one       ));
            push("((2))",          SchemeCons(SchemeCons(two,        SCHEME_NIL), SCHEME_NIL));
            push("((() . 3))",     SchemeCons(SchemeCons(SCHEME_NIL, three     ), SCHEME_NIL));
            push("((2 . 3))",      SchemeCons(SchemeCons(two,        three     ), SCHEME_NIL));
            push("((2 . 3) . 1)",  SchemeCons(SchemeCons(two,        three     ), one       ));

            for (const auto& testWrapper : tests)
            {
                const char* expected = testWrapper.expected;
                const char* actual = testWrapper.cons.toString().GetData();
                Assert::AreEqual(expected, actual);
            }
        }

        TEST_METHOD(ConstructDifferentTypes)
        {
            SchemeInt32 number(42);
            SchemeBool t(SCHEME_TRUE);
            SchemeBool f(SCHEME_FALSE);
            SchemeNil nil(SCHEME_NIL);
            SchemeVoid v(SCHEME_VOID);

            {
                SchemeCons cons1(number);
                SchemeCons cons2(number, number);
            }
            {
                SchemeCons cons1(t);
                SchemeCons cons2(t, t);
            }
            {
                SchemeCons cons1(f);
                SchemeCons cons2(f, f);
            }
            {
                SchemeCons cons1(nil);
                SchemeCons cons2(nil, nil);
            }
            {
                SchemeCons cons1(v);
                SchemeCons cons2(v, v);
            }
        }
    };

}}
