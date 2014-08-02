#include "stdafx.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectUtils.h"

#include "lcpp/core/exceptions/invalidInputException.h"

namespace lcpp
{
    Ptr<LispObject> call(Ptr<LispObject> pCont)
    {
        typeCheck(pCont, Type::Continuation);

        auto pStack = cont::getStack(pCont);

        auto pCallable = pStack->get(-1);

        if(!isCallable(pCallable))
        {
            LCPP_THROW(exceptions::InvalidInput("Argument is not callable."));
        }

        auto& type = pCallable->getType();

        switch(type.getId())
        {
        case Type::Syntax:
            if(isBuiltin(pCallable))
            {
                LCPP_cont_tailCall(pCont, &syntax::builtin::call);
            }
            // TODO support user defined syntax.
            LCPP_NOT_IMPLEMENTED;
        case Type::Lambda:
            if (isBuiltin(pCallable))
            {
                LCPP_cont_tailCall(pCont, &lambda::builtin::call);
            }
            // TODO support user defined lambdas.
            LCPP_NOT_IMPLEMENTED;
        }

        EZ_REPORT_FAILURE("pCallable has the Callable attribute set "
                          "but is not supported in the switch above. "
                          "Did you forget to add code to the switch?");

        return nullptr;
    }

}
