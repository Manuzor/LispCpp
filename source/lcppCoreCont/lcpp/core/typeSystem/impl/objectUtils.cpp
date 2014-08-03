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

    String toString(Ptr<LispObject> pObject)
    {
        EZ_ASSERT(pObject, "Invalid pointer.");

        auto& type = pObject->getType();

        switch(type.getId())
        {
        case Type::Nil: return "Not implemented.";
        case Type::Void: return "Not implemented.";
        case Type::True: return "Not implemented.";
        case Type::False: return "Not implemented.";

        case Type::Integer: return "Not implemented.";
        case Type::Float: return "Not implemented.";

        case Type::Symbol: return "Not implemented.";
        case Type::String: return "Not implemented.";
        case Type::Stream: return "Not implemented.";

        case Type::Cons: return "Not implemented.";
        case Type::Lambda: return "Not implemented.";
        case Type::Syntax: return "Not implemented.";
        case Type::Environment: return "Not implemented.";

        case Type::File: return "Not implemented.";

        case Type::Continuation: return "Not implemented.";
        }

        EZ_REPORT_FAILURE("Unsupported type for toString.");
        LCPP_THROW(exceptions::InvalidInput("Unsupported type for toString."));
    }

}
