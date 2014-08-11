#include "stdafx.h"
#include "lcpp/core/builtins/lambda_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/bool.h"

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/printer.h"

namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            Ptr<LispObject> read(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pToRead = pStack->get(0);

                auto content = ezStringBuilder();

                if (object::isType(pToRead, Type::String))
                {
                    content.Format("(begin %s)", str::getValue(pToRead).GetData());
                }
                else if(object::isType(pToRead, Type::Stream))
                {
                    content.Format("(begin %s)", stream::getIterator(pToRead).GetData());
                }
                else
                {
                    auto message = ezStringBuilder();
                    message.Format("Expected either type \"%s\" or \"%s\", got \"%s\".",
                                   str::metaInfo().getPrettyName().GetData(),
                                   stream::metaInfo().getPrettyName().GetData(),
                                   object::getMetaInfo(pToRead).getPrettyName().GetData());
                    typeCheckFailed(message.GetData());
                }

                auto pContent = str::create(content);
                auto pStream = stream::create(str::getValue(pContent).GetIteratorFront());

                pStack->clear();
                pStack->push(pStream);
                LCPP_cont_tailCall(pCont, &reader::read);
            }

            Ptr<LispObject> eval(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                if(pStack->size() == 1)
                {
                    auto pState = cont::getRuntimeState(pCont);
                    auto pEnv = pState->getGlobalEnvironment();
                    auto pToEval = pStack->get(0);

                    pStack->clear();
                    pStack->push(pEnv);
                    pStack->push(pToEval);
                }

                LCPP_cont_tailCall(pCont, &eval::evaluate);
            }

            Ptr<LispObject> print(Ptr<LispObject> pCont)
            {
                LCPP_cont_tailCall(pCont, &printer::print);
            }
        }
    }
}
