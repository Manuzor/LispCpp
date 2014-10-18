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

            defaultAllocator()->Deallocate(pString->getData<Data>().m_szString);
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

            auto uiSize = value.GetElementCount() + 1; // ez stores the \0 character at the end and we will copy it over.
            auto szString = (char*)defaultAllocator()->Allocate(uiSize, EZ_ALIGNMENT_OF(char*));
            ezMemoryUtils::Copy(szString, value.GetData(), uiSize);

            pInstance->getData<Data>().m_szString = szString;
            return pInstance;
        }

        String getValue(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::String);

            return pObject->getData<Data>().m_szString;
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::String);

            ezStringBuilder theString;
            theString.AppendFormat("\"%s\"", pObject->getData<Data>().m_szString);

            return str::create(theString);
        }

    }
}
