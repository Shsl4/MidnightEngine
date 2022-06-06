#pragma once
#include "PhysX/foundation/PxAllocatorCallback.h"

class PhysXAllocator final : public physx::PxAllocatorCallback {
    
public:

    PhysXAllocator() = default;
    
    virtual ~PhysXAllocator() = default;
    
    virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) override;
    
    virtual void deallocate(void* ptr) override;
    
};
