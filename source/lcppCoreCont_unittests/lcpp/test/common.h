#pragma once

namespace lcpp { namespace test {

    class UnitTest :
        public cut::IUnitTest
    {
    public:

        UnitTest(cut::UnitTestGroup& group);

        void inititialize();
        void shutdown();

        virtual void run() override
        {
            inititialize();
            LCPP_SCOPE_EXIT{ shutdown(); };
            doRun();
        }

        virtual void doRun() = 0;
    };

    class TestStringStream :
        public ezStreamWriterBase
    {
    public:

        ezStringBuilder m_content;

        inline
        virtual ezResult WriteBytes(const void* pWriteBuffer, ezUInt64 uiBytesToWrite) override
        {
            auto pBuffer = reinterpret_cast<const ezUInt8*>(pWriteBuffer);

            for(auto i = ezUInt64(0); i < uiBytesToWrite; ++i)
            {
                m_content.Append(pBuffer[i]);
            }

            return EZ_SUCCESS;
        }

    };

    Ptr<LispObject> readStream(Ptr<LispObject> pStream);

    Ptr<LispObject> readString(const ezString& content);

    Ptr<LispObject> evalStream(Ptr<LispObject> pStream);

    Ptr<LispObject> evalString(const ezString& content);

    Ptr<LispObject> evalObject(Ptr<LispObject> pObject);

}} // namespace lcpp::test

#include "lcpp/test/impl/testGroupMacros.inl"
#include "lcpp/test/impl/testMacros.inl"

#include "lcpp/test/runtime.h"
