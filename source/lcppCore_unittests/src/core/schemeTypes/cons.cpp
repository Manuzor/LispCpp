#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {

    TEST_CLASS(SchemeConsTests)
    {
        TEST_METHOD(Construction)
        {
            // Default construction
            {
                SchemeCons cons;
                Assert::AreEqual<SchemeObject>(cons.car(), SCHEME_NIL,L"");
                Assert::AreEqual<SchemeObject>(cons.cdr(), SCHEME_NIL,L"");
            }

            // Only car given
            {
                SchemeBool t(SCHEME_TRUE);
                SchemeCons cons(t);
                Assert::AreEqual<SchemeObject>(cons.car(), SCHEME_TRUE,L"");
                Assert::AreEqual<SchemeObject>(cons.cdr(), SCHEME_NIL,L"");
            }

            // Car and cdr explicitly given
            {
                SchemeBool t(SCHEME_TRUE);
                SchemeBool f(SCHEME_FALSE);
                SchemeCons cons(t, f);
                Assert::AreEqual<SchemeObject>(cons.car(), SCHEME_TRUE,L"");
                Assert::AreEqual<SchemeObject>(cons.cdr(), SCHEME_FALSE,L"");
            }
        }

        TEST_METHOD(CopyCtorAndCopyAssign)
        {
            {
                SchemeBool t(SCHEME_TRUE);

                SchemeCons first(t);
                SchemeCons second = first;
                Assert::AreEqual<SchemeObject>(first.car(), SCHEME_TRUE);
                Assert::AreEqual<SchemeObject>(second.car(), SCHEME_TRUE, L"");
            }
            {
                SchemeInt32 theInt(42);
                SchemeCons first(theInt);
                SchemeCons second = first;
                Assert::AreEqual<SchemeObject>(first.car(), theInt);
                Assert::AreEqual<SchemeObject>(second.car(), theInt, L"Failed to construct 'second' by assigning it 'first'!");
                Assert::AreSame(first.car(), second.car(), L"Failed to make a flat copy!");
            }
        }
    };

}}
