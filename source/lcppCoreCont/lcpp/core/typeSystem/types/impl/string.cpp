#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/stringData.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/objectData.h"

namespace lcpp
{
    namespace str
    {
        static void destroy(CollectableBase* pCollectable)
        {
            auto pString = static_cast<LispObject*>(pCollectable);
            typeCheck(pString, Type::String);

            pString->getData<Data>().m_string.~String();
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::String);
                meta.setPrettyName("string");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::DestructorFunction,
                                              static_cast<DestructorFunction_t>(&destroy)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create(const String& value)
        {
            LCPP_LogBlock("str::create");

            auto pInstance = object::create<Data>(getMetaInfo());

            pInstance->getData<Data>().m_string = value;

            return pInstance;
        }

        const String& getValue(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::String);

            return pObject->getData<Data>().m_string;
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::String);

            ezStringBuilder theString;
            theString.AppendFormat("\"%s\"", getValue(pObject).GetData());

            return str::create(theString);
        }

    }
}
