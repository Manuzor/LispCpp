#include "stdafx.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectUtils.h"

#include "lcpp/core/exceptions/typeCheckFailedException.h"

namespace lcpp
{
    Ptr<LispObject> call(Ptr<LispObject> pCont)
    {
        typeCheck(pCont, Type::Continuation);

        auto pStack = cont::getStack(pCont);

        auto pCallable = pStack->get(-1);
        auto& type = pCallable->getType();

        switch(type.getId())
        {
        case Type::Syntax:
            // TODO implement me.
            LCPP_NOT_IMPLEMENTED;
            //LCPP_cont_tailCall(pCont, &syntax::call);
        case Type::Lambda:
            LCPP_cont_tailCall(pCont, &lambda::call);
        }

        auto message = ezStringBuilder();
        message.Format("Type check failed, expected either '%s' or '%s' got '%s'.",
                       Type(Type::Syntax).toString(),
                       Type(Type::Lambda).toString(),
                       type.toString());
        LCPP_THROW(exceptions::TypeCheckFailed, message.GetData());
    }

}
