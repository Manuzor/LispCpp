#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"

#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/bool.h"

#include "lcpp/foundation/conversion.h"
#include "lcpp/core/exceptions/arithmeticException.h"
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    namespace number
    {
        static bool isEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            return isTrue(equal(pLhs, pRhs));
        }

        Ptr<const MetaInfo> getMetaInfoInteger()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Integer);
                meta.setPrettyName("integer");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::IsEqualFunction, static_cast<IsEqualFunction_t>(&isEqual)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<const MetaInfo> getMetaInfoFloat()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Float);
                meta.setPrettyName("float");

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create(Integer_t value)
        {
            LCPP_LogBlock("number::create", getMetaInfoInteger()->getPrettyName());

            auto pInstance = object::create<Integer_t>(getMetaInfoInteger());

            pInstance->getData<Integer_t>() = value;

            return pInstance;
        }

        Ptr<LispObject> create(Float_t value)
        {
            LCPP_LogBlock("number::create", getMetaInfoFloat()->getPrettyName());

            auto pInstance = object::create<Float_t>(getMetaInfoFloat());

            pInstance->getData<Float_t>() = value;

            return pInstance;
        }

        Integer_t getInteger(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Integer);

            return pObject->getData<Integer_t>();
        }

        Float_t getFloat(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Float);

            return pObject->getData<Float_t>();
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Integer, Type::Float);

            auto stringValue = String();

            if(object::isType(pObject, Type::Integer))
            {
                stringValue = lcpp::toString(getInteger(pObject));
            }
            else
            {
                stringValue = lcpp::toString(getFloat(pObject));
            }

            return str::create(stringValue.GetData(), stringValue.GetElementCount());
        }

        Ptr<LispObject> negate(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Integer, Type::Float);

            if(object::isType(pObject, Type::Integer))
            {
                return create(-getInteger(pObject));
            }

            return create(-getFloat(pObject));
        }

        Ptr<LispObject> invert(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Integer, Type::Float);

            if(isTrue(isZero(pObject)))
            {
                LCPP_THROW(exceptions::DivisionByZero());
            }

            if(object::isType(pObject, Type::Integer))
            {
                return create(Float_t(1.0f) / getInteger(pObject));
            }

            return create(Float_t(1.0f) / getFloat(pObject));
        }

        Ptr<LispObject> isZero(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Integer, Type::Float);

            if(object::isType(pObject, Type::Integer))
            {
                return getInteger(pObject) == 0 ? LCPP_pTrue : LCPP_pFalse;
            }

            // TODO use epsilon comparison.
            return getFloat(pObject) == 0.0 ? LCPP_pTrue : LCPP_pFalse;
        }

#define LCPP_number_defineArithmeticFunctionContent(pResult, pLhs, pRhs, op) \
    if (object::isType(pLhs, Type::Float))                                   \
    {                                                                        \
        if (object::isType(pRhs, Type::Float))                               \
        {                                                                    \
            pResult = create(getFloat(pLhs) op getFloat(pRhs));              \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            pResult = create(getFloat(pLhs) op getInteger(pRhs));            \
        }                                                                    \
    }                                                                        \
    else                                                                     \
    {                                                                        \
        if (object::isType(pRhs, Type::Float))                               \
        {                                                                    \
            pResult = create(getInteger(pLhs) op getFloat(pRhs));            \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            pResult = create(getInteger(pLhs) op getInteger(pRhs));          \
        }                                                                    \
    }

        Ptr<LispObject> add(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            auto pResult = LCPP_pNil;
            LCPP_number_defineArithmeticFunctionContent(pResult , pLhs, pRhs, +);
            return pResult;
        }

        Ptr<LispObject> subtract(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            auto pResult = LCPP_pNil;
            LCPP_number_defineArithmeticFunctionContent(pResult, pLhs, pRhs, -);
            return pResult;
        }

        Ptr<LispObject> multiply(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            auto pResult = LCPP_pNil;
            LCPP_number_defineArithmeticFunctionContent(pResult, pLhs, pRhs, *);
            return pResult;
        }

        Ptr<LispObject> divide(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            if(isTrue(isZero(pRhs)))
            {
                LCPP_THROW(exceptions::DivisionByZero());
            }

            auto pResult = LCPP_pNil;
            LCPP_number_defineArithmeticFunctionContent(pResult, pLhs, pRhs, /);
            return pResult;
        }

        Ptr<LispObject> modulo(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            if(isTrue(isZero(pRhs)))
            {
                LCPP_THROW(exceptions::DivisionByZero());
            }

            auto pResult = create(getInteger(pLhs) % getInteger(pRhs));
            return pResult;
        }


#undef LCPP_number_defineArithmeticFunctionContent

#define LCPP_number_defineComparisonFunctionContent(pResult, pLhs, pRhs, op)           \
    if (object::isType(pLhs, Type::Float))                                             \
    {                                                                                  \
        if (object::isType(pRhs, Type::Float))                                         \
        {                                                                              \
            pResult = getFloat(pLhs) op getFloat(pRhs) ? LCPP_pTrue : LCPP_pFalse;     \
        }                                                                              \
        else                                                                           \
        {                                                                              \
            pResult = getFloat(pLhs) op getInteger(pRhs) ? LCPP_pTrue : LCPP_pFalse;   \
        }                                                                              \
    }                                                                                  \
    else                                                                               \
    {                                                                                  \
        if (object::isType(pRhs, Type::Float))                                         \
        {                                                                              \
            pResult = getInteger(pLhs) op getFloat(pRhs) ? LCPP_pTrue : LCPP_pFalse;   \
        }                                                                              \
        else                                                                           \
        {                                                                              \
            pResult = getInteger(pLhs) op getInteger(pRhs) ? LCPP_pTrue : LCPP_pFalse; \
        }                                                                              \
    }

        Ptr<LispObject> greaterThan(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            auto pResult = LCPP_pFalse;
            LCPP_number_defineComparisonFunctionContent(pResult, pLhs, pRhs, > );
            return pResult;
        }

        Ptr<LispObject> greaterThanOrEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            auto pResult = LCPP_pFalse;
            LCPP_number_defineComparisonFunctionContent(pResult, pLhs, pRhs, >=);
            return pResult;
        }

        Ptr<LispObject> equal(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            auto pResult = LCPP_pFalse;
            LCPP_number_defineComparisonFunctionContent(pResult, pLhs, pRhs, ==);
            return pResult;
        }

        Ptr<LispObject> lowerThan(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            auto pResult = LCPP_pFalse;
            LCPP_number_defineComparisonFunctionContent(pResult, pLhs, pRhs, <);
            return pResult;
        }

        Ptr<LispObject> lowerThanOrEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            auto pResult = LCPP_pFalse;
            LCPP_number_defineComparisonFunctionContent(pResult, pLhs, pRhs, <=);
            return pResult;
        }

#undef LCPP_number_defineComparisonFunctionContent

    }
}
