#include "stdafx.h"
#include "lcpp/core/memory/garbageCollection.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/typeCheck.h"

namespace lcpp
{
    namespace test
    {
        namespace
        {
            class TestAllocator :
                public ezAllocatorBase
            {
            public:
                struct Allocation
                {
                    EZ_DECLARE_POD_TYPE();

                    void* m_ptr;
                    std::size_t m_uiSize;
                    std::size_t m_uiAlign;
                };

            public:
                ezHashTable<void*, Allocation> m_allocations;
                ezHashTable<void*, Allocation> m_deallocations;
                Array<byte_t> m_protectedMemoryLeft;
                Array<byte_t> m_usableMemory;
                Array<byte_t> m_protectedMemoryRight;
                std::size_t m_uiAllocationPointer;

            public:
                TestAllocator() :
                    m_uiAllocationPointer(0)
                {
                }

                TestAllocator(std::size_t uiProtectedBytesLeft,
                              std::size_t uiUnprotectedSize,
                              std::size_t uiProtectedBytesRight) :
                    m_uiAllocationPointer(0)
                {
                    Initialize(uiProtectedBytesLeft, uiUnprotectedSize, uiProtectedBytesRight);
                }

                ~TestAllocator()
                {
                    Clear();
                }

                void Initialize(std::size_t uiProtectedBytesLeft,
                              std::size_t uiUnprotectedSize,
                              std::size_t uiProtectedBytesRight)
                {
                    const auto uiSize = uiProtectedBytesLeft
                                      + uiUnprotectedSize
                                      + uiProtectedBytesRight;

                    if (uiSize == 0)
                    {
                        return;
                    }

                    auto pMemory = (byte_t*)malloc(uiSize);
                    auto pUsableMemory = pMemory + uiProtectedBytesLeft;
                    auto pProtectedMemoryRight = pUsableMemory + uiProtectedBytesRight;

                    m_protectedMemoryLeft.assign(pMemory, uiProtectedBytesLeft);
                    m_usableMemory.assign(pUsableMemory, uiUnprotectedSize);
                    m_protectedMemoryRight.assign(pProtectedMemoryRight, uiProtectedBytesRight);

                    memset(m_protectedMemoryLeft.getData(),  Byte::Protected,   m_protectedMemoryLeft.getSize());
                    memset(m_usableMemory.getData(),         Byte::Unallocated, m_usableMemory.getSize());
                    memset(m_protectedMemoryRight.getData(), Byte::Protected,   m_protectedMemoryRight.getSize());

                    // At this point, the memory looks like this:
                    // 0x 0ace0ace0ace0ac baaabaaabaaabaaab 0ace0ace0ace0ac
                    //    \_ Protected _/ \_ User Memory _/ \_ Protected _/
                }

                void Clear()
                {
                    if (m_allocations.GetCount() != m_deallocations.GetCount())
                    {
                        CUT_ASSERT.fail("Not enough frees!");
                        return;
                    }

                    free(m_protectedMemoryLeft.getData());
                }

                virtual void* Allocate(std::size_t uiSize, std::size_t uiAlign) override
                {
                    if (uiSize == 0 || uiSize == 42)
                    {
                        return nullptr;
                    }

                    if (m_uiAllocationPointer + uiSize > m_usableMemory.getSize())
                    {
                        // Out of memory!
                        return nullptr;
                    }

                    Allocation allocation{ nullptr, uiSize, uiAlign };
                    allocation.m_ptr = &m_usableMemory[m_uiAllocationPointer];
                    m_uiAllocationPointer += allocation.m_uiSize;

                    memset(allocation.m_ptr, Byte::Allocated, allocation.m_uiSize);

                    m_allocations[allocation.m_ptr] = allocation;

                    return allocation.m_ptr;
                }

                virtual void Deallocate(void* ptr) override
                {
                    if (ptr == nullptr)
                    {
                        return;
                    }

                    if (m_deallocations.KeyExists(ptr))
                    {
                        CUT_ASSERT.fail("Double free!");
                        return;
                    }

                    Allocation* pAllocation(nullptr);

                    if (!m_allocations.TryGetValue(ptr, pAllocation))
                    {
                        CUT_ASSERT.fail("Invalid free!");
                        return;
                    }

                    memset(pAllocation->m_ptr, Byte::Freed, pAllocation->m_uiSize);

                    m_deallocations[ptr] = *pAllocation;
                }

                virtual size_t AllocatedSize(const void*) override
                {
                    return 0;
                }

                virtual Stats GetStats() const override
                {
                    return Stats();
                }

            };

            class TestType : public LispObject
            {
            public:
                static ezHybridArray<Ptr<TestType>, 8> getDestroyedTestTypes()
                {
                    ezHybridArray<Ptr<TestType>, 8> arr(defaultAllocator());
                    return arr;
                }

                number::Integer_t m_integerData;
            };

            void destroy(Ptr<LispObject> pObject)
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

LCPP_TestCase(GarbageCollection, Collect)
{
    TestType::getDestroyedTestTypes().Clear();

    GarbageCollector::CInfo gcCinfo;

    gcCinfo.m_pParentAllocator = defaultAllocator();
    gcCinfo.m_uiInitialMemoryLimit = sizeof(TestType) * 2;

    GarbageCollector gc(gcCinfo);

    auto pRoot = gc.create<TestType>(test::getMetaInfo());
    auto pNonRoot = gc.create<TestType>(test::getMetaInfo());

    gc.addRoot(pRoot);

    CUT_ASSERT.isTrue(gc.isRoot(pRoot));
    CUT_ASSERT.isFalse(gc.isRoot(pNonRoot));

    CUT_ASSERT.isTrue(gc.isAlive(pRoot));
    CUT_ASSERT.isTrue(gc.isAlive(pNonRoot));

    CUT_ASSERT.notImplemented();

    gc.collect();

    CUT_ASSERT.isTrue(gc.isAlive(pRoot));
    CUT_ASSERT.isFalse(gc.isAlive(pNonRoot));
    
    auto& destroyedTestTypes = TestType::getDestroyedTestTypes();
    CUT_ASSERT.isFalse(destroyedTestTypes.Contains(pRoot));
    CUT_ASSERT.isTrue(destroyedTestTypes.Contains(pNonRoot));

    return;
}
