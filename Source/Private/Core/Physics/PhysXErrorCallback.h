#pragma once

#include <PhysX/foundation/PxErrorCallback.h>

class PhysXErrorCallback : public physx::PxErrorCallback {
    
public:
    
    void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
    
};
