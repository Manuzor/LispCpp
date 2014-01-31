#pragma once

namespace lcpp
{
    namespace memory
    {
        class IAllocator
        {
        public:
            virtual ~IAllocator() = 0;

            virtual void* allocate(size_t size) = 0;
            virtual void deallocate(void* mem) = 0;
        };

        class MallocAllocator : public IAllocator
        {
        public:
            inline MallocAllocator(){}
            inline virtual ~MallocAllocator(){}

            inline virtual void* allocate(size_t size) override
            {
                return malloc(size);
            }

            inline virtual void deallocate(void* mem) override
            {
                free(mem);
            }
        };
    }
}