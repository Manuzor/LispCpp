#include "stdafx.h"
#include "lcpp/core/memory/garbageCollection.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

namespace lcpp
{
    namespace test
    {
        namespace
        {
            class TestType : public LispObject
            {
            public:
                static ezHybridArray<StackPtr<TestType>, 8> getDestroyedTestTypes()
                {
                    ezHybridArray<StackPtr<TestType>, 8> arr(defaultAllocator());
                    return arr;
                }

                number::Integer_t m_integerData;
            };

            void destroy(StackPtr<LispObject> pObject)
            {
                typeCheck(pObject, Type::ENUM_COUNT);

                CUT_ASSERT.isFalse(TestType::getDestroyedTestTypes().Contains(pObject));
                TestType::getDestroyedTestTypes().PushBack(pObject);
            }

            Ptr<const MetaInfo> getMetaInfo()
            {
                static auto meta = []{
                    MetaInfo meta;
                    meta.setPrettyName("testType");
                    meta.setType(Type::ENUM_COUNT);
                    meta.addProperty(MetaProperty(MetaProperty::Builtin::DestructorFunction, &destroy));
                    return meta;
                }();

                return &meta;
            }
        }
    }
}

LCPP_TestGroup(GarbageCollection);

LCPP_TestCase(GarbageCollection, FixedMemory)
{
    // No memory.
    {
        FixedMemory stack;

        CUT_ASSERT.isTrue(stack.getAllocationPointer() == 0, "Make sure nothing is allocated for a default-constructed instance.");
        CUT_ASSERT.isTrue(stack.getMemory().getData() == nullptr);
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == 0);
        CUT_ASSERT.isTrue(stack.getEntireMemory() == stack.getMemory());
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocations == 0);
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocatedSize == 0);

        auto* pTemp = (byte_t*)123;
        auto allocationResult = stack.allocate(pTemp);

        CUT_ASSERT.isTrue(allocationResult.isOutOfMemory());
        CUT_ASSERT.isTrue(pTemp == nullptr, "allocate MUST set the out_ value!");
    }

    {
        byte_t rawMemory[1024];
        Array<byte_t> entireMemory(rawMemory);
        FixedMemory stack(entireMemory);

        ezInt32* pInteger = nullptr;
        AllocatorResult result;

        //////////////////////////////////////////////////////////////////////////
        result = stack.allocate(pInteger);

        CUT_ASSERT.isTrue(result.succeeded());
        CUT_ASSERT.isTrue(stack.getAllocationPointer() == sizeof(ezUInt32));
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == sizeof(ezUInt32));
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == stack.getAllocationPointer(), "This should always be true!");
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocatedSize == sizeof(ezUInt32));
        CUT_ASSERT.isTrue(stack.getAvailableMemorySize() == 1024 - stack.getStats().m_uiAllocatedSize);

        //////////////////////////////////////////////////////////////////////////
        result = stack.allocate(pInteger);

        CUT_ASSERT.isTrue(result.succeeded());
        CUT_ASSERT.isTrue(stack.getAllocationPointer() == sizeof(ezUInt32) * 2);
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == sizeof(ezUInt32) * 2);
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == stack.getAllocationPointer(), "This should always be true!");
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocations == 2);
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocatedSize == sizeof(ezUInt32) * 2);
        CUT_ASSERT.isTrue(stack.getAvailableMemorySize() == 1024 - stack.getStats().m_uiAllocatedSize);

        //////////////////////////////////////////////////////////////////////////
        stack.clear();

        CUT_ASSERT.isTrue(stack.getAllocationPointer() == 0);
        CUT_ASSERT.isTrue(stack.getMemory().getData() == nullptr);
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == 0);
        CUT_ASSERT.isTrue(stack.getEntireMemory() == stack.getMemory());
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocations == 0);
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocatedSize == 0);
    }

    {
        char raw0[6];
        char raw1[6];

        strcpy_s(raw0, "hello");
        strcpy_s(raw1, "world");

        Array<byte_t> buffer0(reinterpret_cast<byte_t*>(raw0), 6);
        Array<byte_t> buffer1(reinterpret_cast<byte_t*>(raw1), 6);

        FixedMemory mem0(buffer0);
        FixedMemory mem1(buffer1);

        ezInt16* ptr0;
        ezInt32* ptr1;

        CUT_ASSERT.isTrue(mem0.allocate(ptr0).succeeded());
        CUT_ASSERT.isTrue(mem1.allocate(ptr1).succeeded());

        CUT_ASSERT.isTrue(mem0.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(mem0.getStats().m_uiAllocatedSize == sizeof(ezInt16));
        CUT_ASSERT.isTrue(mem1.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(mem1.getStats().m_uiAllocatedSize == sizeof(ezInt32));

        auto temp = mem0;
        mem0 = mem1;
        mem1 = temp;

        CUT_ASSERT.isTrue(mem0.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(mem0.getStats().m_uiAllocatedSize == sizeof(ezInt32));
        CUT_ASSERT.isTrue(mem1.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(mem1.getStats().m_uiAllocatedSize == sizeof(ezInt16));
    }
}

LCPP_TestCase(GarbageCollection, StackPtr)
{
    GarbageCollector::CInfo gcCinfo;

    gcCinfo.m_uiInitialMemoryLimit = 120 * 1024; // 120 KiB
    gcCinfo.m_pParentAllocator = defaultAllocator();

    GarbageCollector gc(gcCinfo);

    CUT_ASSERT.notImplemented();

    //auto pObject = object::create<number::Integer_t>(&gc, number::getMetaInfoInteger());
    //CUT_ASSERT.isFalse(gc.isOnStack(pObject));
    //
    //{
    //    StackPtr<LispObject> pObjectOnStack = pObject;
    //    CUT_ASSERT.isTrue(gc.isOnStack(pObject));
    //}
    //
    //CUT_ASSERT.isFalse(gc.isOnStack(pObject));
}

LCPP_TestCase(GarbageCollection, Collect)
{
    TestType::getDestroyedTestTypes().Clear();

    GarbageCollector::CInfo gcCinfo;

    gcCinfo.m_uiInitialMemoryLimit = 120 * 1024; // 120 KiB
    gcCinfo.m_pParentAllocator = defaultAllocator();

    GarbageCollector gc(gcCinfo);

    auto pRoot = object::create<cons::Data>(&gc, cons::getMetaInfo());
    auto pRoot_Car = object::create<number::Integer_t>(&gc, number::getMetaInfoInteger());
    pRoot_Car->getData<number::Integer_t>() = 42;
    auto pRoot_Cdr = object::create<number::Integer_t>(&gc, number::getMetaInfoInteger());
    pRoot_Cdr->getData<number::Integer_t>() = 1337;

    new (pRoot->getData<cons::Data>().m_pCar) Ptr<LispObject>(pRoot_Car);
    new (pRoot->getData<cons::Data>().m_pCdr) Ptr<LispObject>(pRoot_Cdr);

    auto pNonRoot = gc.create<TestType>(test::getMetaInfo());

    gc.addRoot(pRoot.get());

    CUT_ASSERT.isTrue(gc.isRoot(pRoot.get()));
    CUT_ASSERT.isFalse(gc.isRoot(pNonRoot.get()));

    CUT_ASSERT.isTrue(gc.isAlive(pRoot.get()));
    CUT_ASSERT.isTrue(gc.isAlive(pNonRoot.get()));

    gc.collect();

    CUT_ASSERT.notImplemented("Check if the collection was successful, somehow.");

    CUT_ASSERT.isTrue(gc.isAlive(pRoot.get()));
    CUT_ASSERT.isFalse(gc.isAlive(pNonRoot.get()));

    auto& destroyedTestTypes = TestType::getDestroyedTestTypes();
    CUT_ASSERT.isFalse(destroyedTestTypes.Contains(pRoot.cast<TestType>()));
    CUT_ASSERT.isTrue(destroyedTestTypes.Contains(pNonRoot.get()));

    return;
}
