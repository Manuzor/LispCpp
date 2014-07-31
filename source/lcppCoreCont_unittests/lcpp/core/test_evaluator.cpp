#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/continuation.h"

namespace lcpp
{
    static Ptr<LispObject> evalStream(Ptr<LispObject> pStream)
    {
        LCPP_NOT_IMPLEMENTED;
    }

    static Ptr<LispObject> evalString(const ezString& content)
    {
        auto pStream = stream::create(content.GetIteratorFront());

        return evalStream(pStream);
    }
}

LCPP_TestGroup(Evaluator);

LCPP_TestCase(Evaluator, EvalEmptyOrWhitespaceString)
{
    auto pObject = Ptr<LispObject>();

    pObject = evalString("");
    CUT_ASSERT.isTrue(isVoid(pObject));

    pObject = evalString("    \n\t\r \t\t\n\r\r\n\r\n   \v\v  \n \t");
    CUT_ASSERT.isTrue(isVoid(pObject));
}
