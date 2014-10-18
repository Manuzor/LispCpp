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

    class UnitTestNoInit :
        public cut::IUnitTest
    {
    public:

        UnitTestNoInit(cut::UnitTestGroup& group);
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

    namespace detail
    {
        extern ezUInt32 g_uiDebugBreakOnExceptions;

        struct DebugBreakOnExceptions
        {
            DebugBreakOnExceptions()  { ++g_uiDebugBreakOnExceptions; }
            ~DebugBreakOnExceptions() { --g_uiDebugBreakOnExceptions; }
        };

        extern ezUInt32 g_uiLogExceptions;

        struct LogExceptions
        {
            LogExceptions()  { ++g_uiDebugBreakOnExceptions; }
            ~LogExceptions() { --g_uiDebugBreakOnExceptions; }
        };
    }

    Ptr<LispObject> readStream(StackPtr<LispObject> pStream);

    Ptr<LispObject> readString(const ezString& content);

    Ptr<LispObject> evalStream(StackPtr<LispObject> pStream);

    Ptr<LispObject> evalString(const ezString& content);

    Ptr<LispObject> evalObject(StackPtr<LispObject> pObject);

}} // namespace lcpp::test

#define LCPP_TEST_DebugBreakOnExceptionsInThisScope ::lcpp::test::detail::DebugBreakOnExceptions EZ_CONCAT(_DebugBreakOnExceptions_, EZ_SOURCE_LINE)
#define LCPP_TEST_LogExceptionsInThisScope ::lcpp::test::detail::LogExceptions EZ_CONCAT(_DebugBreakOnExceptions_, EZ_SOURCE_LINE)

#include "lcpp/test/impl/testGroupMacros.inl"
#include "lcpp/test/impl/testMacros.inl"

#include "lcpp/test/runtime.h"
