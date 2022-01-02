#include <Scene/CameraManager.h>

CameraManager::CameraManager() : cameras(15) { 
    
    
    
}

bool CameraManager::setActiveCamera(CameraComponent* camera){
    
    if(camera && cameras.contains(camera)){
        
        activeCamera = camera;
        return true;
        
    }
    
    return false;
    
}

void CameraManager::registerCamera(CameraComponent* camera){
    
    this->cameras.append(camera);
    
    if(!activeCamera){
        
        activeCamera = camera;
        
    }
    
}

void CameraManager::unregisterCamera(CameraComponent* camera){
    
    this->cameras.remove(camera);
    
    if(cameras.getSize() == 0){
        
        activeCamera = nullptr;
        return;
        
    }
    
    activeCamera = this->cameras[0];
    
}
