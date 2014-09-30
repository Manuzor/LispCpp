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
        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Stream);
                meta.setPrettyName("stream");

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        StackPtr<LispObject> create(ezStringIterator& iterator)
        {
            LCPP_LogBlock("stream::create");

            auto pInstance = object::create<Data>(getMetaInfo());

            auto& data = pInstance->getData<Data>();

            new (data.m_iterator) ezStringIterator(iterator);

            return pInstance;
        }

        ezStringIterator& getIterator(StackPtr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            return pStream->getData<Data>().getIterator();
        }

        void setIterator(StackPtr<LispObject> pStream, ezStringIterator& iter)
        {
            typeCheck(pStream, Type::Stream);

            pStream->getData<Data>().getIterator() = iter;
        }

        ezUInt32 getCharacter(StackPtr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().getIterator();
            return iterator.GetCharacter();
        }

        bool isValid(StackPtr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().getIterator();
            return iterator.IsValid();
        }

        ezUInt32 next(StackPtr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().getIterator();
            ++iterator;
            return iterator.GetCharacter();
        }

        ezUInt32 getPosition(StackPtr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().getIterator();

            if(!iterator.IsValid())
            {
                return EndOfStream;
            }

            auto position = ptrdiff_t(iterator.GetData() - iterator.GetStart());
            return ezUInt32(position);
        }

        StackPtr<LispObject> toString(StackPtr<LispObject> pObject)
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
