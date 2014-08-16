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

namespace lcpp
{
    namespace number
    {
        const MetaInfo& metaInfoInteger()
        {
            static auto meta = MetaInfo(Type::Integer, "integer");
            return meta;
        }

        const MetaInfo& metaInfoFloat()
        {
            static auto meta = MetaInfo(Type::Float, "float");
            return meta;
        }

        Ptr<LispObject> create(Integer_t value)
        {
            auto pInstance = object::create<Integer_t>(metaInfoInteger());

            pInstance->m_integer = value;

            return pInstance;
        }

        Ptr<LispObject> create(Float_t value)
        {
            auto pInstance = object::create<Float_t>(metaInfoFloat());

            pInstance->m_float = value;

            return pInstance;
        }

        Integer_t getInteger(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Integer);

            return pObject->m_integer;
        }

        Float_t getFloat(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Float);

            return pObject->m_float;
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
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

            return str::create(stringValue.GetData());
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
