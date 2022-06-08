#include <Physics/PhysXAllocator.h>
#include <Core/Engine.h>

#ifdef _WIN64

#include <Windows.h>

#else

#include <stdlib.h>

#endif

void* PhysXAllocator::allocate(size_t size, const char* typeName, const char* filename, int line) {

#ifdef _WIN64

    return _aligned_malloc(size, 16);

#elif defined(__APPLE__)
    
    return malloc(size);
    
#else

    return aligned_alloc(size, 16);

#endif
        
}

void PhysXAllocator::deallocate(void* ptr) {

#ifdef _WIN64

    _aligned_free(ptr);

#else

    free(ptr);

#endif
    
}
