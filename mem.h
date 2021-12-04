//
// Created by AAI2002 on 04.12.2021.
//

#ifndef HT3_MEM_H
#define HT3_MEM_H

// Memory leaks detection helpers (to detect leaks for particular test cases)
using namespace std;
int gMemoryLeak = 0;

struct AllocGuard;
AllocGuard* gAllocGuard = nullptr;

#define LOG_ALLOCATIONS 0

struct AllocGuard
{
    AllocGuard()
    {
        gAllocGuard = this;
    }

    ~AllocGuard()
    {
        gMemoryLeak = totalAllocated;
        gAllocGuard = nullptr;
    }

    int totalAllocated = 0;
};


void* operator new(size_t size)
{
    if (gAllocGuard)
    {
        gAllocGuard->totalAllocated += size;
#if LOG_ALLOCATIONS
        std::cout << "+ TA = " << gAllocGuard->totalAllocated << std::endl;
#endif
    }

    void* p = malloc(size + sizeof(size_t));
    size_t* sizePtr = reinterpret_cast<size_t*>(p);
    *sizePtr = size;
    return (char*)p + sizeof(size_t);
}

void operator delete(void* p)
{
    if (gAllocGuard)
    {
        gAllocGuard->totalAllocated -= *(size_t*)((char*)p - sizeof(size_t));
#if LOG_ALLOCATIONS
        std::cout << "- TA = " << gAllocGuard->totalAllocated << std::endl;
#endif
    }

    free((char*)p - sizeof(size_t));
}


#endif //HT3_MEM_H
