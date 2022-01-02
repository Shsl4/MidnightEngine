#pragma once

#include <Object.h>
#include <Memory/Array.h>
#include <Scene/CameraComponent.h>

class CameraManager : public Object{
    
public:
    
    CameraManager();
    
    //FORCEINLINE Array<CameraComponent> getCameras() const { return cameras; }
    FORCEINLINE CameraComponent* getActiveCamera() const { return activeCamera; }
    
    void registerCamera(CameraComponent* camera);
    void unregisterCamera(CameraComponent* camera);
    bool setActiveCamera(CameraComponent* camera);
    
    
protected:
    
    Array<CameraComponent*> cameras;
    Allocator allocator;
    CameraComponent* activeCamera = nullptr;
    
};

