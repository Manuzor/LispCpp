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
        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::Stream, "stream");
            return meta;
        }

        Ptr<LispObject> create(ezStringIterator& iterator)
        {
            auto pInstance = object::create<Data>(metaInfo());

            auto& data = pInstance->m_stream;

            new (data.m_iterator) ezStringIterator(iterator);

            return pInstance;
        }

        ezStringIterator& getIterator(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            return pStream->m_stream.getIterator();
        }

        void setIterator(Ptr<LispObject> pStream, ezStringIterator& iter)
        {
            typeCheck(pStream, Type::Stream);

            pStream->m_stream.getIterator() = iter;
        }

        ezUInt32 getCharacter(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->m_stream.getIterator();
            return iterator.GetCharacter();
        }

        bool isValid(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->m_stream.getIterator();
            return iterator.IsValid();
        }

        ezUInt32 next(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->m_stream.getIterator();
            ++iterator;
            return iterator.GetCharacter();
        }

        ezUInt32 getPosition(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->m_stream.getIterator();

            if(!iterator.IsValid())
            {
                return EndOfStream;
            }

            auto position = ptrdiff_t(iterator.GetData() - iterator.GetStart());
            return ezUInt32(position);
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
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
