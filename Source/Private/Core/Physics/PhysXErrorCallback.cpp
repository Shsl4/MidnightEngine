#include <Physics/PhysXErrorCallback.h>
#include <Console/Console.h>

void PhysXErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) {

    Console::getLogger()->error("PhysX error {}: {} in file {} at line {}", code, message, file, line);
    
}
