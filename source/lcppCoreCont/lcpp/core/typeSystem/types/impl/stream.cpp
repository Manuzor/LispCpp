#include "stdafx.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/streamData.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/objectData.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    namespace stream
    {
        static void destroy(CollectableBase* pCollectable)
        {
            auto pStream = static_cast<LispObject*>(pCollectable);
            typeCheck(pStream, Type::Stream);

            pStream->getData<Data>().m_stringView.~ezStringIterator();
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Stream);
                meta.setPrettyName("stream");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::DestructorFunction,
                                              static_cast<DestructorFunction_t>(&destroy)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create(ezStringIterator& iterator)
        {
            LCPP_LogBlock("stream::create");

            auto pInstance = object::create<Data>(getMetaInfo());

            auto& data = pInstance->getData<Data>();

            data.m_stringView = iterator;

            return pInstance;
        }

        ezStringIterator& getIterator(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            return pStream->getData<Data>().m_stringView;
        }

        void setIterator(Ptr<LispObject> pStream, ezStringIterator& iter)
        {
            typeCheck(pStream, Type::Stream);

            pStream->getData<Data>().m_stringView = iter;
        }

        ezUInt32 getCharacter(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().m_stringView;
            return iterator.GetCharacter();
        }

        bool isValid(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().m_stringView;
            return iterator.IsValid();
        }

        ezUInt32 next(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().m_stringView;
            ++iterator;
            return iterator.GetCharacter();
        }

        ezUInt32 getPosition(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().m_stringView;

            if(!iterator.IsValid())
            {
                return EndOfStream;
            }

            auto position = ptrdiff_t(iterator.GetData() - iterator.GetStart());
            return ezUInt32(position);
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            auto theString = ezStringBuilder();
            theString.Append("<stream at ");

            if (!isValid(pObject))
            {
                theString.Append("end");
            }
            else
            {
                auto position = getPosition(pObject);
                auto currentData = getIterator(pObject).GetData();

                theString.AppendFormat("%u: \"%s\"", position, currentData);
            }

            theString.Append('>');
            return str::create(theString);
        }

    }
}
